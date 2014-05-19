#include "component/density.h"

#include <boost/foreach.hpp>

namespace diagnosis {
namespace component {
t_density_model::t_density_model () {
    weight(1);
}

t_weight t_density_model::weight () const {
    return w;
}

void t_density_model::weight (t_weight weight) {
    this->w = weight;
}

t_density_constant::t_density_constant (t_density density) : density(density) {}

t_density t_density_constant::operator () (const t_state & obs) const {
    return density;
}

t_density t_density_mixture::operator () (const t_state & obs) const {
    t_density density = 0;
    t_weight total_weight = 0;


    BOOST_FOREACH(const t_ptr<t_density_model> &d, densities) {
        density += (* d)(obs);
        total_weight += d->weight();
    }
    return density / total_weight;
}

t_density_mixture & t_density_mixture::operator << (const t_ptr<t_density_model> & density_model) {
    densities.push_back(density_model);
    return *this;
}
}
}