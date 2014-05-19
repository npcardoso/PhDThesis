#ifndef __CANDIDATE_POOL_H_659de5eb48c71894774f310b54b73606ca1f9b94__
#define __CANDIDATE_POOL_H_659de5eb48c71894774f310b54b73606ca1f9b94__

#include "structs/trie.h"
#include <list>
#include <vector>

namespace diagnosis {
namespace structs {
class t_candidate_pool {
public:

    void add (const t_trie & trie);
    void add (const t_candidate & candidate);

    void trie (t_trie & trie) const;

private:
    typedef std::list<t_candidate> t_inner_storage;
    typedef std::vector<t_inner_storage> t_storage;
    t_storage storage;
};
}
}
#endif