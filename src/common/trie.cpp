#include "trie.h"

bool t_trie::add(const t_candidate & candidate, 
                 t_candidate::const_iterator component,
                 bool composites) {

  if(!composites && exists)
    return false;

  if(component == candidate.end()) {
    if(exists)
      return false;
    exists = true;
    elements ++;
    return true;
  }

  t_trie & tmp = children[*component];
  tmp.parent = this;
  if(tmp.add(candidate, ++component, composites)) {
    elements++;
    return true;
  }
  return false;
}

bool t_trie::purge_composites(const t_candidate & candidate,
                              t_candidate::const_iterator component) {
  if(component == candidate.end()) {
    elements = 0;
    exists = false;
    return true;
  }

  t_children::iterator it = children.begin();
  while(it != children.end()) {
    t_candidate::const_iterator tmp = component;
    if(it->first == *component)
      tmp++;
    if(it->first > *component)
      break;

    elements -= it->second.size();
    if(it->second.purge_composites(candidate, tmp))
      children.erase(it);
    elements += it->second.size();
    it++;
  }
  return !exists && !children.size();
}

bool t_trie::is_composite(const t_candidate & candidate,
                          t_candidate::const_iterator component) const {
  if(exists)
    return true;

  while(component != candidate.end()) {
    t_children::const_iterator it = children.find(*component);

    component++;

    if(it == children.end())
      continue;

    if(it->second.is_composite(candidate, component))
      return true;
  }
  return false;
}

void t_trie::add(const t_candidate & candidate, bool composites) {
  if(!composites) {
    if(is_composite(candidate))
      return;
    else
      purge_composites(candidate, candidate.begin());
  }
  add(candidate, candidate.begin(), composites);  
}

std::ostream & t_trie::print(std::ostream & out) const {
  iterator it = begin();
  while(it != end()){
    t_candidate::iterator component = it->begin();
    while(component != it->end())
      out << *(component++) << ",";
    out << std::endl;
    it++;
  }
  return out;  
}

t_trie::iterator t_trie::begin() const {
  return iterator(this);
}

t_trie::iterator t_trie::end() const {
  return iterator(NULL);
}

t_trie_iterator::t_trie_iterator(const t_trie * level): level(level){
  if(level && !level->exists)
    (*this)++;
}

bool t_trie_iterator::operator != (const t_trie_iterator & it) const {
  return !(*this == it);
}

bool t_trie_iterator::operator == (const t_trie_iterator & it) const {
  if(!level && !it.level)
    return true;

  if(!level || !it.level)
    return false;

  return current == it.current;
}

t_trie_iterator t_trie_iterator::operator++(int) {
  assert(level);
  t_trie_iterator ret(*this);
  bool first = false;
  bool first_run = true;
  while(level) {
    /* try to stop */
    if(first && level->exists) 
      break;
    /* go down */
    else if((first || first_run) && 
            level->children.size()){
      current.insert(level->children.begin()->first);
      level = &(level->children.begin()->second);
      first = true;
    }
    /* go up */
    else 
      while(level && (level = level->parent)) {
        t_component_id tmp_component = *current.rbegin();
        current.erase(*current.rbegin());


        t_trie::t_children::const_iterator it = level->children.find(tmp_component);
        if((++it) != level->children.end()) {
          current.insert(it->first);
          level = &(it->second);
          first = true;
          break;
        }

      }
    first_run = false;
  }

  return ret;
}

const t_candidate * t_trie_iterator::operator->() const {
  assert(level);
  return &current;
}

const t_candidate & t_trie_iterator::operator*() const {
  assert(level);
  return current;
}
