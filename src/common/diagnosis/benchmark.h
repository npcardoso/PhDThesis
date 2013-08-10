#ifndef __DIAGNOSIS_BENCHMARK_H__
#define __DIAGNOSIS_BENCHMARK_H__

#include "diagnosis/diagnosis_system.h"
#include "diagnosis/metrics.h"
#include "diagnosis/rank_element.h"
#include "diagnosis/randomizers/randomizer.h"
#include "diagnosis/structs/candidate.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>
#include <boost/random/mersenne_twister.hpp>

namespace diagnosis {
namespace metrics {
template <class S>
class t_benchmark : public t_diagnosis_system {
public:
    typedef S t_spectra_type;
    typedef typename randomizers::t_spectra_randomizer<t_spectra_type>::t_ptr t_randomizer_type;

    t_benchmark (const t_candidate_generator::t_ptr & gen,
                 const t_candidate_ranker::t_ptr & rank,
                 const t_randomizer_type & rand) : t_diagnosis_system(gen, rank), randomizer(rand) {}

    t_diagnosis_cost operator () (boost::random::mt19937 & gen) const {
        t_spectra_type spectra;
        structs::t_candidate correct;
        t_candidate_generator::t_ret_type D;
        t_candidate_ranker::t_ret_type probs;


        (* randomizer)(spectra, correct, gen);

        (* generator)(spectra, D);
        (* ranker)(spectra, D, probs);


        t_probability total = 0;
        t_candidate_generator::t_ret_type::iterator it = D.begin();
        t_candidate_ranker::t_ret_type::iterator it_prob = probs.begin();

        std::list<structs::t_candidate> D_list;
        typedef diagnosis::t_rank_element<const structs::t_candidate *, t_probability> t_rank_element_tmp;
        typedef std::vector<t_rank_element_tmp> t_rank;
        t_rank rank;

        while (it != D.end()) {
            total += *it_prob;
            D_list.push_front(*it);
            rank.push_back(t_rank_element_tmp(&(D_list.front()), *it_prob));
            it++;
        }

        sort(rank.begin(), rank.end());
        return Cd_multi(correct, rank);
    }

protected:
    t_randomizer_type randomizer;
};
}
}
#endif