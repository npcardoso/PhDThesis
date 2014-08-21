#include "bernoulli.h"
#include <boost/random/bernoulli_distribution.hpp>

#include "../../structs/count_spectrum.h"

using boost::bernoulli_distribution;

namespace diagnostic {
namespace benchmark {

t_bernoulli::t_bernoulli (float activation_rate,
                          float error_rate,
                          t_count n_tran,
                          t_count n_comp) {
    this->activation_rate = activation_rate;
    this->error_rate = error_rate;
    this->n_comp = n_comp;
    this->n_tran = n_tran;
}

t_spectrum * t_bernoulli::operator () (std::mt19937 & gen,
                                      t_candidate & correct_candidate) {
    t_count_spectrum & spectrum = *(new t_count_spectrum(n_comp, n_tran));


    bernoulli_distribution<> activation(activation_rate);
    bernoulli_distribution<> error(error_rate);

    correct_candidate.clear();

    for (t_transaction_id t = 1; t <= n_tran; t++) {
        bool success = false;
        spectrum.set_error(t, error(gen));

        while (!success)
            for (t_component_id c = 1; c <= n_comp; c++) {
                bool active = activation(gen);
                spectrum.set_activations(c, t, active);
                success = active || success;
            }
    }

    return &spectrum;
}

std::ostream & t_bernoulli::write (std::ostream & out) const {
    out << "t_bernoulli(" << activation_rate << ", ";
    out << error_rate << ", " << n_tran << ", " << n_comp << ")";
    return out;
}
}
}
