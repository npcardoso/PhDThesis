#include "similarity_creator.h"

#include "../algorithms/similarity.h"

namespace diagnostic {
namespace configuration {

static t_algorithm_creator::t_register registration(t_const_ptr<t_algorithm_creator>(new t_similarity_creator()));

t_const_ptr<t_candidate_ranker> t_similarity_creator::ranker(const t_algorithm & a) const {
    t_const_ptr<t_candidate_ranker> ret;
    const t_algorithm::t_configs & configs = a.get_configs();

    if(a.get_name() != "similarity")
        return ret;

    t_algorithm::t_configs::const_iterator it = configs.find("type");
    if(it == configs.end())
        return ret;

    std::string type = it->second;
    if(type == "ochiai")
        ret = t_const_ptr<t_candidate_ranker>(new algorithms::t_ochiai());
    else if(type == "jaccard")
        ret = t_const_ptr<t_candidate_ranker>(new algorithms::t_jaccard());

    return ret;
}
}
}
