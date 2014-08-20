#include "ranking.h"

#include "utils/iostream.h"

#include <algorithm>
#include <numeric>
#include <boost/foreach.hpp>
using namespace std;

namespace diagnostic {
t_ranking::t_ranking (const t_candidate_generator::t_ret_type & candidates,
                      const t_candidate_ranker::t_ret_type & scores,
                      t_score_type score_type) {
    t_candidates * cc = new t_candidates();
    BOOST_FOREACH(const auto & c, candidates) {
        cc->push_back(c);
    }
    this->candidates = t_ptr<t_candidates>(cc);
    init(scores, score_type);
}

t_ranking::t_ranking (t_const_ptr<t_candidates> candidates,
                      const t_candidate_ranker::t_ret_type & scores,
                      t_score_type score_type) {
    this->candidates = candidates;
    init(scores, score_type);
}

const std::vector<t_rank_element> & t_ranking::get_elements() const {
    return rank;
}

t_entropy t_ranking::get_entropy () const {
    return entropy;
}

t_score t_ranking::get_normalization_factor() const {
    return normalization_factor;
}

t_score_type t_ranking::get_score_type() const {
    return score_type;
}

t_const_ptr<t_ranking::t_candidates> t_ranking::get_candidates() const {
    return t_const_ptr<t_candidates>(candidates);
}

void t_ranking::init (const t_candidate_ranker::t_ret_type & scores,
                      t_score_type score_type) {
    assert(candidates.get() != NULL);
    assert(candidates->size() == scores.size());

    this->score_type = score_type;

    normalization_factor = accumulate(scores.begin(), scores.end(), 0);

    entropy = 0;

    t_candidates::const_iterator it_cand = candidates->begin();
    BOOST_FOREACH(const auto & score, scores) {
        rank.push_back(t_rank_element (*it_cand, score));

        if (score> 0)
            entropy -= score * log(score);
    }

    sort(rank.begin(), rank.end(), std::greater<t_rank_element> ());
}

}

namespace std {
std::ostream & operator << (std::ostream & out,
                            const diagnostic::t_ranking & r) {
    BOOST_FOREACH(const diagnostic::t_rank_element & element,
                  r.get_elements()) {
        out << element << std::endl;
    }

    return out;
}
}
