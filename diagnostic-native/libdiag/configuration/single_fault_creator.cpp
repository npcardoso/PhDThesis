#include "single_fault_creator.h"
#include "../algorithms/single_fault.h"

using namespace diagnostic::algorithms;

namespace diagnostic {
namespace configuration {

static t_algorithm_creator::t_register registration(t_const_ptr<t_algorithm_creator>(new t_single_fault_creator()));

t_const_ptr<t_candidate_generator> t_single_fault_creator::generator(const t_algorithm & a) const {
    t_single_fault * generator = NULL;

    if(a.get_name() == "single_fault") {
        generator = new t_single_fault();
    }

    return t_const_ptr<t_candidate_generator>(generator);
}
}
}
