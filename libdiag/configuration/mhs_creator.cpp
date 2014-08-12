#include "mhs_creator.h"

#include "../algorithms/mhs.h"

namespace diagnostic {
namespace configuration {

static t_algorithm_creator::t_register registration(t_const_ptr<t_algorithm_creator>(new t_mhs_creator()));

t_const_ptr<t_candidate_generator> t_mhs_creator::generator(const t_algorithm & a) const {
    t_const_ptr<t_candidate_generator> ret;
    //const t_algorithm::t_configs & configs = a.get_configs();

    if(a.get_name() != "mhs")
        return ret;
    ret = t_const_ptr<t_candidate_generator>(new algorithms::t_mhs());
    //! TODO: Add all options
    return ret;
}
}
}
