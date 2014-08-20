#include "cutoff.h"

#include <sstream>

using namespace std;

namespace diagnostic {
namespace algorithms {
bool t_basic_cutoff::stop (t_count depth) const {
    return false;
}

bool t_basic_cutoff::stop (const t_rank & rank,
                           t_id pos,
                           const t_trie & D,
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
                     const t_trie & D,
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

void t_cutoff::json_configs (t_configs & out) const {
    out["max_depth"] = std::to_string(max_depth);
    out["max_candidates"] = std::to_string(max_candidates);
    out["max_time"] = std::to_string(max_time);
    out["lambda"] = std::to_string(lambda);
    out["min_score"] = std::to_string(min_score);
}
}
}
