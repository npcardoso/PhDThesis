#include "qos.h"
#include <boost/random/bernoulli_distribution.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
using namespace boost::random;
using namespace diagnosis::structs;

namespace diagnosis {
namespace randomizers {
t_fuzzy_bernoulli::t_fuzzy_bernoulli  (float activation_rate, t_count error_count) : activation_rate(activation_rate), error_count(error_count), component_count(0) {}

const t_fuzzy_bernoulli::t_self_type & t_fuzzy_bernoulli::operator () (structs::t_count_spectra & spectra,
                                                                       t_candidate & correct_candidate,
                                                                       boost::random::mt19937 & gen) const {
    assert(!(error_count > 0 && faulty_components.size() == 0));


    bernoulli_distribution<> activation(activation_rate);

    if (component_count)
        spectra.set_component_count(component_count);

    spectra.set_transaction_count(0);


    correct_candidate.clear();
    t_count errors = 0;

    for (t_transaction_id t = 1; errors < error_count; t++) {
        spectra.new_transaction();

        for (t_component_id c = 1; c <= spectra.get_component_count(); c++) {
            spectra.set_count(c, t, activation(gen));
        }

        for (t_component_id c = 1; c <= faulty_components.size(); c++) {
            if (!spectra.get_count(c, t))
                continue;

            t_error e = faulty_components[c].gen_error(gen);
            spectra.set_error(t, std::max(e, spectra.get_error(t)));

            if (e == 1)
                correct_candidate.insert(c + 1);
        }

        if (spectra.is_error(t))
            errors++;
    }

    return *this;
}

t_fuzzy_bernoulli & t_fuzzy_bernoulli::operator << (const structs::t_fault & comp) {
    faulty_components.push_back(comp);
    return *this;
}
}
}