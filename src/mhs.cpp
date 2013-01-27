#include "spectra.h"
#include "count_spectra.h"
#include "similarity.h"
#include "spectra_filter.h"
#include "spectra_iterator.h"

#include "trie.h"

template <class T_ACTIVITY>
class t_mhs {

  const t_heuristic<T_ACTIVITY> & heuristic;

  bool all_failed(t_component_id component, 
                  const t_spectra <T_ACTIVITY> & spectra,
                  const t_spectra_filter & filter) const {

    t_spectra_iterator it(spectra.get_component_count(),
                          spectra.get_transaction_count(),
                          &filter);
    it.set_component(component);

    while (it.next_transaction()){
      bool activity = spectra.get_count(it.get_component(), it.get_transaction());
      bool error = spectra.is_error(it.get_transaction());
      if(error && !activity)
        return false;
    }
    return true;
  }

  void strip(t_component_id component,
             const t_spectra <T_ACTIVITY> & spectra,
             t_spectra_filter & filter) const {

    t_spectra_iterator it(spectra.get_component_count(),
                          spectra.get_transaction_count(),
                          &filter);
    it.set_component(component);

    while (it.next_transaction()){
      bool activity = spectra.get_count(it.get_component(), it.get_transaction());
      if(activity)
        filter.filter_transaction(it.get_transaction());
    }
    filter.filter_component(component);
  }

public:
  t_count max_candidate_size, max_candidates;
  float heuristic_cutoff;
  t_mhs(const t_heuristic<T_ACTIVITY> & heuristic):  heuristic(heuristic){
    heuristic_cutoff = 0.000000001;
  }

  void calculate(const t_spectra <T_ACTIVITY> & spectra,
                 t_trie & D,
                 const t_spectra_filter * filter = NULL) const {
    
    t_spectra_filter tmp_filter;
    if(filter)
      tmp_filter = *filter;

    /* Removing singleton candidates */

    t_spectra_iterator it(spectra.get_component_count(),
                          spectra.get_transaction_count(),
                          &tmp_filter);

    while(it.next_component())
      if(all_failed(it.get_component(), spectra, tmp_filter)) {
        t_candidate tmp_candidate;
        tmp_candidate.insert(it.get_component());
        D.add(tmp_candidate);
        
        tmp_filter.filter_component(it.get_component());
      }

    /* Ranking */

    t_order_buffer order_buffer = spectra.get_ordering_buffer(&tmp_filter);

    heuristic.order(spectra, &tmp_filter, order_buffer.get());

    /* Creating complex candidates */

    t_count remaining_components = spectra.get_component_count() - tmp_filter.get_filtered_component_count();

    for(t_id i = 0; i < remaining_components; i++) {
      if(order_buffer[i].get_value() <= heuristic_cutoff)
        break;

      t_spectra_filter strip_filter = tmp_filter;
      t_trie partial_D;

      strip(order_buffer[i].get_component(), spectra, strip_filter);
      calculate(spectra, partial_D, &strip_filter);

      /* Append partial candidates with current component */
      t_trie::iterator it = partial_D.begin();

      while(it != partial_D.end()) {
        t_candidate tmp_candidate = *(it++);
        tmp_candidate.insert(order_buffer[i].get_component());
        D.add(tmp_candidate);
      }

    }
  }
};
