#include "trie.h"

#include <cassert>

namespace diagnosis {
namespace structs {
std::ostream & operator << (std::ostream & out, const t_trie & trie) {
    return trie.print(out);
}

std::istream & operator >> (std::istream & in, t_trie & trie) {
    while (true) {
        t_trie::t_value_type c;
        in >> c;

        if (!c.size())
            break;

        trie.add(c);
    }

    in.clear();
    return in;
}

bool t_trie::add (const t_value_type & candidate,
                  t_value_type::const_iterator component,
                  bool composites) {
    if (!composites && exists)
        return false;

    if (component == candidate.end()) {
        if (exists)
            return false;

        exists = true;
        elements++;
        return true;
    }

    t_trie & tmp = children[*component];
    tmp.parent = this;

    if (tmp.add(candidate, ++component, composites)) {
        elements++;
        return true;
    }

    return false;
}

bool t_trie::purge_composites (const t_value_type & candidate,
                               t_value_type::const_iterator component) {
    if (component == candidate.end()) {
        elements = 0;
        exists = false;
        return true;
    }

    t_children::iterator it = children.begin();

    while (it != children.end()) {
        t_value_type::const_iterator tmp = component;

        if (it->first == *component)
            tmp++;

        if (it->first > *component)
            break;

        elements -= it->second.size();
        bool remove = it->second.purge_composites(candidate, tmp);
        elements += it->second.size();

        if (remove)
            children.erase(it++);
        else
            it++;
    }

    return !exists && !children.size();
}

bool t_trie::is_composite (const t_value_type & candidate,
                           t_value_type::const_iterator component,
                           bool strict) const {
    if (exists && (!strict || component != candidate.end()))
        return true;

    while (component != candidate.end()) {
        t_children::const_iterator it = children.find(*component);

        component++;

        if (it == children.end())
            continue;

        if (it->second.is_composite(candidate, component))
            return true;
    }

    return false;
}

bool t_trie::add (const t_value_type & candidate,
                  bool purge_composites,
                  bool check_composite) {
    if (check_composite && is_composite(candidate))
        return false;

    if (purge_composites)
        this->purge_composites(candidate, candidate.begin());

    return add(candidate, candidate.begin(), check_composite);
}

std::ostream & t_trie::print (std::ostream & out) const {
    iterator it = begin();


    while (it != end())
        out << *(it++) << std::endl;

    return out;
}

t_trie::iterator t_trie::begin () const {
    return iterator(this);
}

t_trie::iterator t_trie::end () const {
    return iterator(NULL);
}

bool t_trie::operator == (const t_trie & t) const {
    iterator my = begin(), other = t.begin();


    while (my != end() && other != t.end()) {
        if (*my != *other)
            return false;

        my++;
        other++;
    }

    return (my == end() && other == t.end());
}

t_trie_iterator::t_trie_iterator (const t_trie * level) : level(level) {
    if (level && !level->exists)
        (*this)++;
}

bool t_trie_iterator::operator != (const t_trie_iterator & it) const {
    return !(*this == it);
}

bool t_trie_iterator::operator == (const t_trie_iterator & it) const {
    if (!level && !it.level)
        return true;

    if (!level || !it.level)
        return false;

    return current == it.current;
}

t_trie_iterator t_trie_iterator::operator ++ () {
    return (*this)++;
}

t_trie_iterator t_trie_iterator::operator ++ (int) {
    assert(level);
    t_trie_iterator ret(*this);
    bool first = false;
    bool first_run = true;

    while (level) {
        /* try to stop */
        if (first && level->exists)
            break;
        /* go down */
        else if ((first || first_run) &&
                 level->children.size()) {
            current.insert(level->children.begin()->first);
            level = &(level->children.begin()->second);
            first = true;
        }
        /* go up */
        else
            while (level && (level = level->parent)) {
                t_component_id tmp_component = *current.rbegin();
                current.erase(*current.rbegin());


                t_trie::t_children::const_iterator it = level->children.find(tmp_component);

                if ((++it) != level->children.end()) {
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

const t_trie::t_value_type * t_trie_iterator::operator -> () const {
    assert(level);
    return &current;
}

const t_trie::t_value_type & t_trie_iterator::operator * () const {
    assert(level);
    return current;
}
}
}