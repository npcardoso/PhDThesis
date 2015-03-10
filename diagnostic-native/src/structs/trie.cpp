#include "trie.h"

#include <cassert>
#include <boost/foreach.hpp>
namespace diagnostic {
std::ostream & operator << (std::ostream & out,
                            const t_trie & trie) {
    return trie.write(out);
}

std::istream & operator >> (std::istream & in,
                            t_trie & trie) {
    while (true) {
        t_trie::t_value_type c;
        in >> c;

        if (!c.size())
            break;

        trie.add(c);
    }

    return in;
}

t_trie::t_trie () {
    exists = false;
    parent = NULL;
    elements = 0;
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

void t_trie::clear () {
    children.clear();
    exists = false;
    elements = 0;
}

bool t_trie::is_composite (const t_value_type & candidate,
                           bool strict) const {
    return is_composite(candidate, candidate.begin(), strict);
}

t_count t_trie::size () const {
    return elements;
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

std::ostream & t_trie::generic_write (std::ostream & out,
                                      std::string prefix,
                                      std::string suffix,
                                      std::string separator,
                                      std::string cand_prefix,
                                      std::string cand_suffix,
                                      std::string cand_separator) const {
    t_group group;


    group.open(out, prefix, suffix, separator);
    BOOST_FOREACH(const auto & el, *this) {
        group.element(out);
        el.generic_write(out, cand_prefix, cand_suffix, cand_separator);
    }
    group.close(out);

    return out;
}

std::ostream & t_trie::write (std::ostream & out) const {
    return generic_write(out,
                         TRIE_NORMAL_PREFIX,
                         TRIE_NORMAL_SUFFIX,
                         TRIE_NORMAL_SEP,
                         CANDIDATE_NORMAL_PREFIX,
                         CANDIDATE_NORMAL_SUFFIX,
                         CANDIDATE_NORMAL_SEP);
}

std::ostream & t_trie::pretty_write (std::ostream & out) const {
    return generic_write(out,
                         TRIE_PRETTY_PREFIX,
                         TRIE_PRETTY_SUFFIX,
                         TRIE_PRETTY_SEP,
                         CANDIDATE_PRETTY_PREFIX,
                         CANDIDATE_PRETTY_SUFFIX,
                         CANDIDATE_PRETTY_SEP);
}

std::ostream & t_trie::latex_write (std::ostream & out) const {
    return generic_write(out,
                         TRIE_LATEX_PREFIX,
                         TRIE_LATEX_SUFFIX,
                         TRIE_LATEX_SEP,
                         CANDIDATE_LATEX_PREFIX,
                         CANDIDATE_LATEX_SUFFIX,
                         CANDIDATE_LATEX_SEP);
}

std::ostream & t_trie::json (std::ostream & out) const {
    return json_write_container(out, *this, "[", "]");
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