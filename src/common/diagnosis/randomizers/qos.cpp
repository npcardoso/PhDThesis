#include "qos.h"
#include <boost/random/bernoulli_distribution.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
using namespace boost::random;
using namespace diagnosis::structs;

namespace diagnosis {
namespace randomizers {
t_faulty_component::t_faulty_component (t_error e_min, t_error e_max) {
    assert(e_min >= 0);
    assert(e_max >= 0);
    assert(e_min <= 1);
    assert(e_max <= 1);
    this->e_min = e_min;
    this->e_max = e_max;
}

t_error t_faulty_component::gen_error (boost::random::mt19937 & gen) const {
    assert(e_min < e_max);

    uniform_real_distribution<t_error> error(e_min, e_max);

    return error(gen);
}

t_qos_randomizer::t_qos_randomizer  (float activation_rate, float error_rate) : t_bernoulli_randomizer(activation_rate, error_rate) {}

void t_qos_randomizer::operator () (structs::t_count_spectra & spectra,
                                    t_candidate & correct_candidate,
                                    boost::random::mt19937 & gen) const {
    assert(!(error_rate > 0 && faulty_components.size() == 0));


    bernoulli_distribution<> activation(activation_rate);
    bernoulli_distribution<> error(error_rate);

    if (n_comp)
        spectra.set_component_count(n_comp);

    if (n_tran)
        spectra.set_transaction_count(n_tran);

    correct_candidate.clear();

    for (t_transaction_id t = 1; t <= n_tran; t++) {
        for (t_component_id c = 1; c <= n_comp; c++) {
            spectra.set_count(c, t, activation(gen));
        }

        if (error(gen)) {
            uniform_int_distribution<t_component_id> erroneous(0, faulty_components.size() - 1);
            t_error e = 0;
            t_component_id c = erroneous(gen);

            spectra.set_count(c + 1, t, 1);
            e = faulty_components[c].gen_error(gen);
            spectra.set_error(t, e);

            if (spectra.is_error(t))
                correct_candidate.insert(c + 1);
        }
    }
}

t_qos_randomizer & t_qos_randomizer::operator << (const t_faulty_component & comp) {
    faulty_components.push_back(comp);
    return *this;
}
}
}