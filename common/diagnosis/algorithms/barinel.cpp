#include "barinel.h"
#include "utils/iostream.h"

namespace diagnosis {
namespace algorithms {

  t_barinel_model::t_barinel_model():
    pass(1, 0), fail(1, 0) {

    }

  t_barinel_model::t_barinel_model(size_t components) {
    set_size(components);
  }

  void t_barinel_model::set_size(size_t components) {
    pass.resize(1 << components, 0);
    fail.resize(1 << components, 0);
  }

  t_barinel::t_barinel() {
    g_j = 0.001;
    epsilon = 0.0001;
    lambda = 0.0001;
    precision = 128;
    iterations = 100000;
  }

  t_barinel::t_barinel(size_t precision) {
    g_j = 0.001;
    epsilon = 0.0001;
    lambda = 0.0001;
    iterations = 100000;
    this->precision = precision;
  }

  void t_barinel::calculate(const t_spectra & spectra,
                            const t_candidate & candidate,
                            t_probability_mp & ret,
                            const t_spectra_filter * filter) const {
    t_barinel_model m(candidate.size());
    model(spectra, candidate, m, filter);
    t_barinel_goodnesses g(candidate.size(), t_goodness_mp(0.5, precision)),
                         old_g(candidate.size(), t_goodness_mp(0.5, precision));

    t_probability_mp pr(0, precision),
                     old_pr(0, precision);

    for(t_count it = 0; it < iterations; it++) {

      //Update goodness values;
      for(t_id c = 0; c < candidate.size(); c++){
        t_goodness_mp delta(0, precision);
        gradient(m, old_g, c, delta);
        g[c] += lambda * delta;
        if(g[c] <= 0)
          g[c] = 0.00001;
        else if(g[c] >= 1)
          g[c] = 1 - 0.00001;
      }
      old_g = g;

      //Calculate probability
      spectra.probability(candidate, g, pr, filter);

      //Check stop conditions

      if(abs(pr - old_pr) < epsilon)
        break;
      old_pr = pr;
    }
    
    t_probability_mp prior_pr;
    prior(candidate, prior_pr);

    ret = prior_pr * pr;
  }


  void t_barinel::model(const t_spectra & spectra,
                        const t_candidate & candidate,
                        t_barinel_model & model,
                        const t_spectra_filter * filter) const {

    assert(candidate.size() < sizeof(t_component_id) * 8);

    t_spectra_filter tmp_filter;
    if(filter)
      tmp_filter = *filter;

    tmp_filter.resize_components(spectra.get_component_count());
    tmp_filter.filter_all_components_but(candidate);

    t_spectra_iterator it(spectra.get_component_count(),
                          spectra.get_transaction_count(),
                          &tmp_filter);

    model.set_size(candidate.size());
    while (it.next_transaction()) {
      t_id symbol = 0;

      for(t_id comp = 0; it.next_component(); comp++) {
        if(spectra.get_count(it.get_component(),
                             it.get_transaction()))
          symbol += 1 << comp;
      }

      model.pass[symbol] += 1 - spectra.get_error(it.get_transaction());
      model.fail[symbol] += spectra.get_error(it.get_transaction());
    }
  }


  void t_barinel::gradient(const t_barinel_model & model,
                           const t_barinel_goodnesses & goodnesses,
                           t_id component,
                           t_goodness_mp & ret) const {
    assert(component < goodnesses.size());
    t_goodness_mp tmp_f(0, precision);
                 

    ret = 0;

    for(t_id pattern = 0; pattern < (1 << (goodnesses.size() - 1)); pattern++) {
      //Symbol id calculation
      t_id symbol = (1UL << component) |
        (pattern & ((1UL << component) - 1)) |
        ((pattern << 1) & ~((1UL << (component + 1)) - 1));

      //PASS stuff
      ret += model.pass[symbol] / goodnesses[component];

      //FAIL stuff
      tmp_f = 1;

      for(t_id i = 0; i < goodnesses.size(); i++)
        if(symbol & (1 << i))
          tmp_f *= goodnesses[i];
      
      ret += -model.fail[symbol] * (tmp_f / goodnesses[component]) / (1 - tmp_f);
    }
  }

    void t_barinel::prior(const t_candidate & candidate,
                          t_goodness_mp & ret) const {
      ret = pow(g_j, candidate.size());
    }
}
}
