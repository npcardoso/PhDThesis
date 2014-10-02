#include "similarity_creator.h"

using namespace diagnostic::algorithms;

namespace diagnostic {
namespace configuration {
// static t_algorithm_creator::t_register registration(t_const_ptr<t_algorithm_creator>(new t_similarity_creator()));

t_similarity * t_similarity_creator::get_ranker (const std::string & type) {
    if (type == "ochiai")
        return new t_ochiai();
    else if (type == "jaccard")
        return new t_jaccard();

    return NULL;
}

t_const_ptr<t_candidate_ranker> t_similarity_creator::ranker (const t_algorithm & a) const {
    t_candidate_ranker * ranker = NULL;


    if (a.get_name() == "similarity") {
        const std::string * type = get(a, "type");

        if (type != NULL)
            ranker = t_similarity_creator::get_ranker(*type);
    }

    return t_const_ptr<t_candidate_ranker> (ranker);
}
}
}