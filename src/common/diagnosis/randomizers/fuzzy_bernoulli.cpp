#include "fuzzy_bernoulli.h"
#include <boost/random/bernoulli_distribution.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
using namespace boost::random;
using namespace diagnosis::structs;

namespace diagnosis {
namespace randomizers {
t_fuzzy_bernoulli::t_fuzzy_bernoulli  (t_count component_count, float activation_rate, t_count error_count) : component_count(component_count), activation_rate(activation_rate), error_count(error_count) {}

structs::t_spectra * t_fuzzy_bernoulli::operator () (boost::random::mt19937 & gen,
                                                     structs::t_candidate & correct_candidate) const {
    assert(!(error_count > 0 && faulty_components.size() == 0));

    bernoulli_distribution<> activation(activation_rate);
    t_count_spectra & spectra = *(new t_count_spectra(component_count, 0));


    correct_candidate.clear();
    t_count errors = 0;

    for (t_transaction_id t = 1; errors < error_count; t++) {
        spectra.new_transaction();

        for (t_component_id c = 1; c <= spectra.get_component_count(); c++) {
            spectra.set_activations(c, t, activation(gen));
        }

        for (t_component_id c = 1; c <= faulty_components.size(); c++) {
            if (!spectra.get_activations(c, t))
                continue;

            t_error e = faulty_components[c].gen_error(gen);
            spectra.set_error(t, std::max(e, spectra.get_error(t)));

            if (e == 1)
                correct_candidate.insert(c + 1);
        }

        if (spectra.is_error(t))
            errors++;
    }

    return &spectra;
}

t_fuzzy_bernoulli & t_fuzzy_bernoulli::operator << (const structs::t_fault & comp) {
    faulty_components.push_back(comp);
    component_count++;
    return *this;
}
}
}