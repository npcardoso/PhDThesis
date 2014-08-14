#include "algorithm_creator_combiner.h"

#include <boost/foreach.hpp>

namespace diagnostic {
namespace configuration {

t_const_ptr<t_candidate_generator> t_algorithm_creator_combiner::generator(const t_algorithm & a) const {
    t_const_ptr<t_candidate_generator> ret;
    BOOST_FOREACH(const t_creators::value_type & e, creators) {
        ret = e->generator(a);
        if(ret)
            return ret;
    }
    return ret;
}

t_const_ptr<t_candidate_ranker> t_algorithm_creator_combiner::ranker(const t_algorithm & a) const {
    t_const_ptr<t_candidate_ranker> ret;
    BOOST_FOREACH(const t_creators::value_type & e, creators) {
        ret = e->ranker(a);
        if(ret)
            return ret;
    }
    return ret;
}

void t_algorithm_creator_combiner::add(t_const_ptr<t_algorithm_creator> c) {
    creators.push_back(c);
}
}
}
