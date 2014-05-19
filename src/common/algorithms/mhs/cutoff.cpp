#include "cutoff.h"

namespace diagnosis {
namespace algorithms {
bool t_basic_cutoff::stop (t_count depth) const {
    return false;
}

bool t_basic_cutoff::stop (const t_rank & rank,
                           t_id pos,
                           const structs::t_trie & D,
                           t_time_interval time_elapsed) const {
    return false;
}

t_cutoff::t_cutoff () {
    max_candidates = 0;
    max_depth = 0;
    max_time = -1;
    lambda = 1;
    min_score = 0;
}

bool t_cutoff::stop (t_count depth) const {
    if (max_depth && depth > max_depth)
        return true;

    return false;
}

bool t_cutoff::stop (const t_rank & rank,
                     t_id pos,
                     const structs::t_trie & D,
                     t_time_interval time_elapsed) const {
    if (max_candidates && D.size() >= max_candidates)
        return true;

    if (max_time > 0 && time_elapsed > max_time)
        return true;

    if (rank.get_score(pos) < min_score)
        return true;

    if ((pos / (float) rank.size()) > lambda)
        return true;

    return false;
}
}
}