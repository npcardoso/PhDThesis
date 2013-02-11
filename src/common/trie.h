#ifndef __TRIE_H__
#define __TRIE_H__

#include "candidate.h"
#include "types.h"

#include <map>
#include <boost/lexical_cast.hpp>

class t_trie_iterator;

class t_trie {
  typedef std::map<t_component_id, t_trie> t_children;

  const t_trie * parent;
  t_children children;
  bool exists;

  t_count elements;

  bool add(const t_candidate & candidate, 
           t_candidate::const_iterator component,
           bool composites);

  bool purge_composites(const t_candidate & candidate,
                        t_candidate::const_iterator component);

  bool is_composite(const t_candidate & candidate,
                    t_candidate::const_iterator component,
                    bool strict = false) const;

public:
  typedef t_trie_iterator iterator;

  inline t_trie() {
    exists = false;
    parent = NULL;
    elements = 0;
  }

  void add(const t_candidate & candidate, 
           bool purge_composites = true,
           bool check_composite = true);

  inline bool is_composite(const t_candidate & candidate,
                           bool strict = false) const {
    return is_composite(candidate, candidate.begin(), strict);  
  }

  inline t_count size() const {
    return elements;
  }

  std::ostream & print(std::ostream & out) const;

  iterator begin() const;
  iterator end() const;
  
  inline void clear() {
    children.clear();
    exists = false;
  }

  friend class t_trie_iterator;
};

std::ostream & operator << (std::ostream & out, const t_trie & trie);
std::istream & operator >> (std::istream & in, t_trie & trie);


class t_trie_iterator {
  t_candidate current;
  const t_trie * level;
public:

  t_trie_iterator(const t_trie * level);
  bool operator != (const t_trie_iterator & it) const;
  bool operator == (const t_trie_iterator & it) const;

  t_trie_iterator operator++(int);

  const t_candidate * operator->() const;
  const t_candidate & operator*() const;
};

#endif
