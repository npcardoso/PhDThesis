#include "barinel.h"

namespace diagnosis {
namespace algorithms {
using namespace structs;

t_barinel_model::t_barinel_model () : pass(1, 0), fail(1, 0) {}

t_barinel_model::t_barinel_model (size_t components) {
    set_size(components);
}

void t_barinel_model::set_size (size_t components) {
    pass.resize(1 << components, 0);
    fail.resize(1 << components, 0);
}

t_barinel::t_barinel () {
    g_j = 0.001;
    epsilon = 0.0001;
    lambda = 0.0001;
    precision = 128;
    iterations = 100000;
    use_fuzzy_error = true;
    use_confidence = true;
}

t_barinel::t_barinel (size_t precision) {
    g_j = 0.001;
    epsilon = 0.0001;
    lambda = 0.0001;
    iterations = 0;
    this->precision = precision;
}

void t_barinel::operator () (const structs::t_spectra & spectra,
                             const structs::t_trie & D,
                             t_ret_type & probs,
                             const structs::t_spectra_filter * filter) const {
    t_trie::iterator it = D.begin();


    while (it != D.end()) {
        t_probability_mp ret;
        calculate(spectra, *it, ret);
        probs.push_back(ret.toDouble());
        it++;
    }
}

void t_barinel::calculate (const t_spectra & spectra,
                           const t_candidate & candidate,
                           t_probability_mp & ret,
                           const t_spectra_filter * filter) const {
    t_barinel_model m(candidate.size());


    model(spectra, candidate, m, filter);

    t_barinel_goodnesses g(candidate.size(), t_goodness_mp(0.5, precision));
    t_barinel_goodnesses grad(candidate.size(), t_goodness_mp(0.5, precision));

    t_probability_mp pr(0, precision),
    old_pr(0, precision);
    t_count it = 0;

    while (true) {
        if (iterations && ++it > iterations)
            break;

        // Update goodness values;
        gradient(m, g, grad);

        for (t_id c = 0; c < candidate.size(); c++) {
            g[c] += lambda * grad[c];

            if (g[c] <= 0)
                g[c] = 0.00001;
            else if (g[c] >= 1)
                g[c] = 1 - 0.00001;
        }


        // Calculate probability
        spectra.probability(candidate, g, pr, filter, use_confidence, use_fuzzy_error);

        // Check stop conditions
        // if ((pr - old_pr) < epsilon)
        if (2 * (pr - old_pr) / (pr + old_pr) < epsilon)
            break;

        old_pr = pr;
    }

    t_probability_mp prior_pr;
    prior(candidate, prior_pr);

    ret = prior_pr * pr;
}

void t_barinel::model (const t_spectra & spectra,
                       const t_candidate & candidate,
                       t_barinel_model & model,
                       const t_spectra_filter * filter) const {
    assert(candidate.size() < sizeof(t_component_id) * 8);

    t_spectra_filter tmp_filter;

    if (filter)
        tmp_filter = *filter;

    tmp_filter.resize_components(spectra.get_component_count());
    tmp_filter.filter_all_components_but(candidate);

    t_spectra_iterator it(spectra.get_component_count(),
                          spectra.get_transaction_count(),
                          &tmp_filter);

    model.set_size(candidate.size());

    while (it.next_transaction()) {
        t_id symbol = 0;

        for (t_id comp = 0; it.next_component(); comp++) {
            if (spectra.get_count(it.get_component(),
                                  it.get_transaction()))
                symbol += 1 << comp;
        }

        t_confidence conf = use_confidence ? spectra.get_confidence(it.get_transaction()) : 1;
        t_error err = use_fuzzy_error ? spectra.get_error(it.get_transaction()) : spectra.is_error(it.get_transaction());
        model.pass[symbol] += conf * (1 - err);
        model.fail[symbol] += conf * err;
    }
}

void t_barinel::gradient (const t_barinel_model & model,
                          const t_barinel_goodnesses & goodnesses,
                          t_barinel_goodnesses & ret) const {
    t_goodness_mp tmp_f(0, precision);


    ret = t_barinel_goodnesses(goodnesses.size(), t_goodness_mp(0, precision));

    for (t_id pattern = 0; pattern < (1 << (goodnesses.size())); pattern++) {
        // PASS stuff
        for (t_id c = 0; c < goodnesses.size(); c++)
            ret[c] += model.pass[pattern] / goodnesses[c];

        // FAIL stuff
        tmp_f = 1;

        t_id pattern_tmp = pattern;

        for (t_id c = 0; pattern_tmp; c++, pattern_tmp >>= 1)
            if (pattern_tmp & 1)
                tmp_f *= goodnesses[c];

        pattern_tmp = pattern;

        for (t_id c = 0; pattern_tmp; c++, pattern_tmp >>= 1)
            if (pattern_tmp & 1)
                ret[c] += -model.fail[pattern] * (tmp_f / goodnesses[c]) / (1 - tmp_f);
    }
}

void t_barinel::prior (const t_candidate & candidate,
                       t_goodness_mp & ret) const {
    ret = pow(g_j, candidate.size());
}
}
}