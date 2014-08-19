#include "algorithm_creator.h"

#include "algorithm_creator_combiner.h"


namespace diagnostic {
namespace configuration {

static t_algorithm_creator_combiner DEFAULT;

t_algorithm_creator::t_register::t_register(const t_const_ptr<t_algorithm_creator> ac) {
    DEFAULT.add(ac);
}


const t_algorithm_creator & t_algorithm_creator::get_default() {
    return DEFAULT;
}


t_const_ptr<t_candidate_generator> t_algorithm_creator::generator(const t_algorithm & a) const {
    return t_const_ptr<t_candidate_generator>();
}

t_const_ptr<t_candidate_ranker> t_algorithm_creator::ranker(const t_algorithm & a) const {
    return t_const_ptr<t_candidate_ranker>();
}

const std::string * t_algorithm_creator::get(const t_algorithm & a,
                                       const std::string & key) const {
    const t_algorithm::t_configs & configs = a.get_configs();
    t_algorithm::t_configs::const_iterator it = configs.find(key);

    if(it == configs.end())
        return NULL;

    return &(it->second);
}

}
}
