#include "algorithm_creator.h"

#include "mhs_creator.h"
#include "fuzzinel_creator.h"
#include "similarity_creator.h"
#include "single_fault_creator.h"
#include "algorithm_creator_combiner.h"


namespace diagnostic {
namespace configuration {
// static t_algorithm_creator_combiner DEFAULT;

// t_algorithm_creator::t_register::t_register(const t_const_ptr<t_algorithm_creator> ac) {
// DEFAULT.add(ac);
// }

t_ptr<t_algorithm_creator> t_algorithm_creator::get_default () {
    t_algorithm_creator_combiner * tmp = new t_algorithm_creator_combiner();


    tmp->add(t_const_ptr<t_algorithm_creator> (new t_single_fault_creator()));
    tmp->add(t_const_ptr<t_algorithm_creator> (new t_mhs_creator()));
    tmp->add(t_const_ptr<t_algorithm_creator> (new t_fuzzinel_creator()));
    tmp->add(t_const_ptr<t_algorithm_creator> (new t_similarity_creator()));
    return t_ptr<t_algorithm_creator> (tmp);
}

t_const_ptr<t_candidate_generator> t_algorithm_creator::generator (const t_algorithm & a) const {
    return t_const_ptr<t_candidate_generator> ();
}

t_const_ptr<t_candidate_ranker> t_algorithm_creator::ranker (const t_algorithm & a) const {
    return t_const_ptr<t_candidate_ranker> ();
}

const std::string * t_algorithm_creator::get (const t_algorithm & a,
                                              const std::string & key) const {
    const t_algorithm::t_configs & configs = a.get_configs();
    t_algorithm::t_configs::const_iterator it = configs.find(key);


    if (it == configs.end())
        return NULL;

    return &(it->second);
}
}
}