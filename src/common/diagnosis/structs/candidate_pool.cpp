#include "candidate_pool.h"

#include <boost/foreach.hpp>


namespace diagnosis {
namespace structs {
void t_candidate_pool::add (const t_trie & trie) {
    BOOST_FOREACH(const t_candidate &c, trie) {
        add(c);
    }
}

void t_candidate_pool::add (const t_candidate & candidate) {
    if (storage.size() < candidate.size())
        storage.resize(candidate.size());

    storage[candidate.size() - 1].push_back(candidate);
}

void t_candidate_pool::trie (t_trie & trie) const {
    BOOST_FOREACH(const t_storage::value_type & l, storage) {
        BOOST_FOREACH(const t_inner_storage::value_type & c, l) {
            trie.add(c, false, true);
        }
    }
}
}
}