#include "fuzzinel_creator.h"

#include "../algorithms/fuzzinel.h"

#include <boost/lexical_cast.hpp>

using namespace boost;
using namespace diagnostic::algorithms;
using namespace std;

namespace diagnostic {
namespace configuration {
// static t_algorithm_creator::t_register registration(t_const_ptr<t_algorithm_creator>(new t_fuzzinel_creator()));

t_const_ptr<t_candidate_ranker> t_fuzzinel_creator::ranker (const t_algorithm & a) const {
    t_const_ptr<t_candidate_ranker> ret;

    if (a.get_name() != "fuzzinel")
        return ret;

    t_fuzzinel * fuzzinel = new t_fuzzinel();

    const string * str;

    if ((str = get(a, "use_fuzzy_error")) != NULL)
        fuzzinel->use_fuzzy_error = lexical_cast<bool> (*str);

    if ((str = get(a, "use_confidence")) != NULL)
        fuzzinel->use_confidence = lexical_cast<bool> (*str);

    ret = t_ptr<t_candidate_ranker> (fuzzinel);

    return ret;
}
}
}