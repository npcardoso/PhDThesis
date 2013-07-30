#include "goodness.h"

#include "exceptions.h"

#include <boost/foreach.hpp>

namespace diagnosis {
namespace component {
void t_goodness_model::label (t_label_id lbl,
                              t_density_model::t_ptr model,
                              bool good) {
    assert(lbl < labels.size());
    this->labels[lbl] = model;
    this->good[lbl] = good;
}

t_label_id t_goodness_model::label (t_density_model::t_ptr model,
                                    bool good) {
    this->labels.push_back(model);
    this->good.push_back(good);
    return labels.size() - 1;
}

const t_density_model::t_ptr & t_goodness_model::label (t_label_id lbl) const {
    assert(lbl < labels.size());
    return labels[lbl];
}

t_count t_goodness_model::label_count () const {
    return labels.size();
}

t_probability t_goodness_model::operator () (const t_state & obs, t_label_id lbl) const {
    assert(lbl < labels.size());

    t_density total = 0;
    t_density good_total = (*labels[lbl])(obs) * labels[lbl]->weight();

    BOOST_FOREACH(const t_density_model::t_ptr & l,
                  labels)
    total += (* l)(obs) * l->weight();
    return good_total / total;
}

t_probability t_goodness_model::operator () (const t_state & obs) const {
    t_density total = 0;
    t_density good_total = 0;

    t_id i = 0;


    BOOST_FOREACH(const t_density_model::t_ptr & l,
                  labels) {
        t_probability tmp = (* l)(obs) * l->weight();


        total += tmp;

        if (good[i++])
            good_total += tmp;
    }
    return good_total / total;
}
}
}