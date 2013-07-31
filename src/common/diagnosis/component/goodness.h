#ifndef __COMMON_DIAGNOSIS_COMPONENT_GOODNESS_H__
#define __COMMON_DIAGNOSIS_COMPONENT_GOODNESS_H__

#include "diagnosis/component/density.h"

#include <boost/random/mersenne_twister.hpp>

namespace diagnosis {
namespace component {
typedef float t_probability;

class t_probability_model {
public:
    DEFINE_BOOST_SHARED_PTRS(t_probability_model);

    virtual t_density operator () (const t_state & obs) const = 0;
};

class t_goodness_model : public t_probability_model {
public:
    virtual void label (t_label_id lbl,
                        t_density_model::t_ptr model,
                        bool good);
    virtual t_label_id label (t_density_model::t_ptr model,
                              bool good);

    virtual const t_density_model::t_ptr& label (t_label_id lbl) const;
    virtual t_count label_count () const;

    virtual t_probability operator () (const t_state & obs) const = 0;
    virtual t_probability operator () (const t_state & obs, t_label_id label) const;


private:
    typedef std::vector < t_density_model::t_ptr >t_labels;
    t_labels labels;

    typedef std::vector < bool >t_good;
    t_good good;
};
}
}

#endif