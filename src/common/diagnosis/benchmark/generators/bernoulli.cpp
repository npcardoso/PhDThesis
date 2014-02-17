#include "bernoulli.h"
#include <boost/random/bernoulli_distribution.hpp>

using boost::bernoulli_distribution;
using namespace diagnosis::structs;

namespace diagnosis {
namespace benchmark {
using namespace structs;

t_bernoulli::t_bernoulli (float activation_rate,
                          float error_rate,
                          t_count n_tran,
                          t_count n_comp) {
    this->activation_rate = activation_rate;
    this->error_rate = error_rate;
    this->n_comp = n_comp;
    this->n_tran = n_tran;
}

t_spectra * t_bernoulli::operator () (std::mt19937 & gen,
                                      structs::t_candidate & correct_candidate) {
    t_count_spectra & spectra = *(new t_count_spectra(n_comp, n_tran));


    bernoulli_distribution<> activation(activation_rate);
    bernoulli_distribution<> error(error_rate);

    correct_candidate.clear();

    for (t_transaction_id t = 1; t <= n_tran; t++) {
        spectra.set_error(t, error(gen));

        for (t_component_id c = 1; c <= n_comp; c++) {
            spectra.set_activations(c, t, activation(gen));
        }
    }

    return &spectra;
}

std::ostream & t_bernoulli::write (std::ostream & out) const {
    out << "t_bernoulli(" << activation_rate << ", ";
    out << error_rate << ", " << n_tran << ", " << n_comp << ")";
    return out;
}
}
}