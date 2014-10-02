#include "fuzzy_bernoulli.h"

#include "../../structs/count_spectrum.h"


#include <boost/random/bernoulli_distribution.hpp>

using namespace boost::random;

namespace diagnostic {
namespace benchmark {
t_fuzzy_bernoulli::t_fuzzy_bernoulli  (t_count component_count,
                                       float activation_rate,
                                       t_count error_count) : activation_rate(activation_rate), component_count(component_count), error_count(error_count) {}

t_spectrum * t_fuzzy_bernoulli::operator () (std::mt19937 & gen,
                                             t_candidate & correct_candidate) {
    assert(!(error_count > 0 && faulty_components.size() == 0));

    bernoulli_distribution<> activation(activation_rate);
    t_count_spectrum & spectrum = *(new t_count_spectrum(component_count, 0));


    correct_candidate.clear();
    t_count errors = 0;

    for (t_transaction_id t = 1; errors < error_count; t++) {
        spectrum.new_transaction();

        for (t_component_id c = 1; c <= spectrum.get_component_count(); c++) {
            spectrum.set_activations(c, t, activation(gen));
        }

        for (t_component_id c = 1; c <= faulty_components.size(); c++) {
            if (!spectrum.get_activations(c, t))
                continue;

            t_error e = faulty_components[c].gen_error(gen);
            spectrum.set_error(t, std::max(e, spectrum.get_error(t)));

            if (e == 1)
                correct_candidate.insert(c + 1);
        }

        if (spectrum.is_error(t))
            errors++;
    }

    return &spectrum;
}

t_fuzzy_bernoulli & t_fuzzy_bernoulli::operator << (const t_fault & comp) {
    faulty_components.push_back(comp);
    component_count++;
    return *this;
}

std::ostream & t_fuzzy_bernoulli::write (std::ostream & out) const {
    out << "t_fuzzy_bernoulli(" << activation_rate << ", ";
    out << error_count << ", " << error_count << ", " << faulty_components.size() << ")";
    return out;
}
}
}