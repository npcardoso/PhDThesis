#include "mhs_creator.h"

#include "similarity_creator.h"

#include <boost/lexical_cast.hpp>

using namespace boost;
using namespace diagnostic::algorithms;
using namespace std;

namespace diagnostic {
namespace configuration {
// static t_algorithm_creator::t_register registration(t_const_ptr<t_algorithm_creator>(new t_mhs_creator()));

t_mhs_creator::t_mhs_creator () : default_cutoff(new t_cutoff()), default_similarity(new t_ochiai()) {
    default_cutoff->max_time = 60e6; // 60 seconds cutoff as default
}

t_const_ptr<t_candidate_generator> t_mhs_creator::generator (const t_algorithm & a) const {
    t_const_ptr<t_candidate_generator> ret;

    if (a.get_name() != "mhs2")
        return ret;

    t_const_ptr<t_similarity> similarity = default_similarity;
    t_ptr<t_cutoff> cutoff(new t_cutoff(*default_cutoff));
    const string * str;

    if ((str = get(a, "max_depth")) != NULL)
        cutoff->max_depth = lexical_cast<t_count> (*str);

    if ((str = get(a, "max_candidates")) != NULL)
        cutoff->max_candidates = lexical_cast<t_count> (*str);

    if ((str = get(a, "max_time")) != NULL)
        cutoff->max_time = lexical_cast<t_time_interval> (*str);

    if ((str = get(a, "lambda")) != NULL)
        cutoff->lambda = lexical_cast<float> (*str);

    if ((str = get(a, "min_score")) != NULL)
        cutoff->min_score = lexical_cast<t_score> (*str);

    if ((str = get(a, "heuristic")) != NULL)
        similarity = t_const_ptr<t_similarity> (t_similarity_creator::get_ranker(*str));


    t_mhs * mhs = new t_mhs();
    mhs->set_cutoff(cutoff);
    mhs->set_similarity(similarity);

    ret = t_ptr<t_candidate_generator> (mhs);

    return ret;
}
}
}