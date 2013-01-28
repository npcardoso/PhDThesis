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

  bool add(const t_candidate & candidate, 
           t_candidate::const_iterator component,
           bool composites);

  bool purge_composites(const t_candidate & candidate,
                        t_candidate::const_iterator component);

  bool is_composite(const t_candidate & candidate,
                    t_candidate::const_iterator component) const;

public:
  typedef t_trie_iterator iterator;

  inline t_trie() {
    exists = false;
    parent = NULL;
  }

  void add(const t_candidate & candidate, bool composites=false);

  inline bool is_composite(const t_candidate & candidate) const {
    return is_composite(candidate, candidate.begin());  
  }

  std::ostream & print(std::ostream & out) const;

  iterator begin() const;
  iterator end() const;

  friend class t_trie_iterator;
};

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
