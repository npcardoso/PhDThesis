#include "kernel.h"

#include <boost/math/distributions/normal.hpp>
#include <boost/foreach.hpp>
namespace diagnosis {
namespace component {
t_density t_kernel_uniform::operator () (const t_state & obs) const {
    BOOST_FOREACH(float v, obs)

    if (v * v > 1)
        return 0;

    return 0.5;
}

t_density t_kernel_gaussian::operator () (const t_state & obs) const {
// return(boost::math::pdf(boost::math::normal(), obs));
    return 0;
}
}
}