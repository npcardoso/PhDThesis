#ifndef __COMMON_DIAGNOSIS_COMPONENT_DENSITY_H__
#define __COMMON_DIAGNOSIS_COMPONENT_DENSITY_H__

#include "types.h"
#include "diagnosis/component/state.h"
#include "utils/boost.h"

#include <list>

namespace diagnosis {
namespace component {
typedef float t_density;
typedef float t_weight;

class t_density_model {
public:
    DEFINE_BOOST_SHARED_PTRS(t_density_model);

    t_density_model ();

    t_weight weight () const;
    void weight (t_weight weight);

    virtual t_density operator () (const t_state & obs) const = 0;

private:
    t_weight w;
};

class t_density_constant : public t_density_model {
public:
    t_density_constant (t_density density);

    virtual t_density operator () (const t_state & obs) const;
private:
    t_density density;
};

class t_density_mixture : public t_density_model {
public:
    virtual t_density operator () (const t_state & obs) const;

    t_density_mixture & operator << (const t_density_model::t_ptr & density_model);

private:
    std::list < t_density_model::t_ptr >densities;
};
}
}

#endif