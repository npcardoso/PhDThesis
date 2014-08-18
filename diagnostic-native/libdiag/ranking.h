#ifndef __RANKING_H_9122f5d28fe5cbf4669865eae3fb40687848669c__
#define __RANKING_H_9122f5d28fe5cbf4669865eae3fb40687848669c__

#include "candidate.h"
#include "diagnostic_system.h"
#include "rank_element.h"

#include <vector>
#include <list>


namespace diagnostic {
class t_ranking {
    public:
    typedef std::list<t_candidate> t_candidates;

    t_ranking (const t_candidate_generator::t_ret_type & candidates,
               const t_candidate_ranker::t_ret_type & scores,
               t_score_type score_type);

    t_ranking (t_const_ptr<t_candidates> candidates,
               const t_candidate_ranker::t_ret_type & scores,
               t_score_type score_type);

    const std::vector<t_rank_element> & get_elements() const;

    t_entropy get_entropy () const;
    t_score get_normalization_factor() const;
    t_score_type get_score_type() const;

    t_const_ptr<t_candidates> get_candidates() const;

    private:


    void init (const t_candidate_ranker::t_ret_type & scores,
               t_score_type score_type);

    t_entropy entropy;
    t_score normalization_factor;
    t_score_type score_type;
    std::vector<t_rank_element> rank;

    t_const_ptr<t_candidates> candidates;
};
}


namespace std {
std::ostream & operator << (std::ostream & out,
                            const diagnostic::t_ranking & r);
}


#endif
