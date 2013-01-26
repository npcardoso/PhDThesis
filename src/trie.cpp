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
    return true;
  }

  return children[*component].add(candidate, 
                                  ++component,
                                  composites);
}

bool t_trie::purge_composites(const t_candidate & candidate,
                              t_candidate::const_iterator component) {
  if(component == candidate.end()) {
    exists = false;
    return true;
  }
  
  t_children::iterator it = children.begin();
  while(it != children.end()) {
    t_candidate::const_iterator tmp = component;
    if(it->first == *component)
      tmp++;

    if(it->second.purge_composites(candidate, tmp))
      children.erase(it++);
    else
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
