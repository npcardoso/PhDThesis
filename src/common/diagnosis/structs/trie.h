#ifndef __TRIE_H_037b1d52aa2e47a40fa8548c585fcab5ed49afef__
#define __TRIE_H_037b1d52aa2e47a40fa8548c585fcab5ed49afef__

#include "types.h"
#include "diagnosis/structs/candidate.h"

#include <map>
#include <boost/lexical_cast.hpp>

namespace diagnosis {
namespace structs {
class t_trie_iterator;

class t_trie {
public:
    typedef t_candidate t_value_type;
    typedef t_value_type value_type; // Compatibility with stl
    typedef t_trie_iterator t_iterator;
    typedef t_iterator iterator; // Compatibility with stl
    typedef t_iterator const_iterator; // Compatibility with stl

    inline t_trie () {
        exists = false;
        parent = NULL;
        elements = 0;
    }

    void add (const t_value_type & candidate,
              bool purge_composites=true,
              bool check_composite=true);

    inline bool is_composite (const t_value_type & candidate,
                              bool strict=false) const {
        return is_composite(candidate, candidate.begin(), strict);
    }

    inline t_count size () const {
        return elements;
    }

    std::ostream & print (std::ostream & out) const;

    iterator begin () const;
    iterator end () const;

    inline void clear () {
        children.clear();
        exists = false;
        elements = 0;
    }

    bool operator == (const t_trie & t);
    friend class t_trie_iterator;

private:
    bool add (const t_value_type & candidate,
              t_value_type::const_iterator component,
              bool composites);

    bool purge_composites (const t_value_type & candidate,
                           t_value_type::const_iterator component);

    bool is_composite (const t_value_type & candidate,
                       t_value_type::const_iterator component,
                       bool strict=false) const;

private:
    typedef std::map<t_component_id, t_trie> t_children;

    const t_trie * parent;
    t_children children;
    bool exists;

    t_count elements;
};

std::ostream & operator << (std::ostream & out, const t_trie & trie);
std::istream & operator >> (std::istream & in, t_trie & trie);


class t_trie_iterator {
    t_trie::t_value_type current;
    const t_trie * level;
public:

    t_trie_iterator (const t_trie * level);
    bool operator != (const t_trie_iterator & it) const;
    bool operator == (const t_trie_iterator & it) const;


    // Warning: one of those must be buggy for statements like *(it++)
    t_trie_iterator operator ++ ();
    t_trie_iterator operator ++ (int);

    const t_trie::t_value_type * operator -> () const;
    const t_trie::t_value_type & operator * () const;
};
}
}

#endif