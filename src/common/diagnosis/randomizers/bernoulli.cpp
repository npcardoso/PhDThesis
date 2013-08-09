#include "bernoulli.h"
#include <boost/random/bernoulli_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
using boost::bernoulli_distribution;

namespace diagnosis {
namespace randomizers {
using namespace structs;

t_bernoulli_randomizer::t_bernoulli_randomizer (float activation_rate, float error_rate) {
    this->activation_rate = activation_rate;
    this->error_rate = error_rate;
    this->n_comp = 0;
    this->n_tran = 0;
}

void t_bernoulli_randomizer::randomize (t_count_spectra & spectra) const {
    boost::random::mt19937 gen;


    bernoulli_distribution<> activation(activation_rate);
    bernoulli_distribution<> error(error_rate);

    if (n_comp)
        spectra.set_component_count(n_comp);

    if (n_tran)
        spectra.set_transaction_count(n_tran);

    for (t_transaction_id t = 1; t <= n_tran; t++) {
        spectra.set_error(t, error(gen));

        for (t_component_id c = 1; c <= n_comp; c++) {
            spectra.set_count(c, t, activation(gen));
        }
    }
}
}
}