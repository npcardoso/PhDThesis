#ifndef __MHS_H__
#define __MHS_H__

#include "../heuristic/heuristic.h"
#include "../spectra/count_spectra.h"
#include "../spectra/spectra.h"
#include "../spectra/spectra_filter.h"
#include "../spectra/spectra_iterator.h"
#include "../common/trie.h"
#include "../common/utils.h"

#include <map>

template <class T_ACTIVITY>
class t_mhs {
  typedef std::map<t_count, t_heuristic<T_ACTIVITY> > t_heuristics;

  t_heuristics heuristics;

public:
  t_count max_candidate_size, max_candidates;
  t_time_interval max_time;

  t_mhs(const t_heuristic<T_ACTIVITY> & heuristic) {
    max_candidate_size = 0;
    max_candidates = 0;
    max_time = 0;
    set_heuristic(0, heuristic);
  }

  void set_heuristic(t_count start_level, const t_heuristic<T_ACTIVITY> & heuristic) {
    heuristics[start_level] = heuristic;
  }

  void calculate(const t_spectra <T_ACTIVITY> & spectra,
                 t_trie & D,
                 const t_spectra_filter * filter = NULL) const {

    t_candidate candidate;
    calculate(spectra, D, filter, candidate, get_time_interval());

  }

  void calculate(const t_spectra <T_ACTIVITY> & spectra,
                 t_trie & D,
                 const t_spectra_filter * filter,
                 t_candidate & candidate,
                 t_time_interval start_time) const {

    t_spectra_filter tmp_filter;
    if(filter)
      tmp_filter = *filter;

    /* Candidate Length cutoff */

    if(max_candidate_size && candidate.size() + 1 > max_candidate_size)
      return;

    /* Removing singleton candidates */

    {
      t_spectra_iterator it(spectra.get_component_count(),
                            spectra.get_transaction_count(),
                            &tmp_filter);

      while(it.next_component())
        if(all_failed(it.get_component(), spectra, tmp_filter)) {
          std::pair<t_candidate::iterator, bool> tmp = candidate.insert(it.get_component());
          assert(tmp.second);

          /* Result Length cutoff */

          if(max_candidate_size && D.size() >= max_candidates)
            return;

          D.add(candidate);
          candidate.erase(tmp.first);

          tmp_filter.filter_component(it.get_component());
        }
    }

    /* Candidate Length cutoff */

    if(max_candidate_size && candidate.size() + 2 > max_candidate_size)
      return;

    /* Ranking */

    t_order_buffer order_buffer = spectra.get_ordering_buffer(&tmp_filter);

    get_heuristic(candidate.size())(spectra, order_buffer.get(), &tmp_filter);

    /* Creating complex candidates */

    t_count remaining_components = spectra.get_component_count() - tmp_filter.get_filtered_component_count();

    for(t_id i = 0; i < remaining_components; i++) {
      t_component_id component = order_buffer[i].get_component();
      
      /* Result Length cutoff */
      if(max_candidate_size && D.size() >= max_candidates)
        return;

      /* Time cutoff */
      if(max_time > 0 && get_time_interval() - start_time > max_time)
        return;

      /* Heuristic cutoff */
      if(component == 0)
        break;


      /* Heuristic signalling skip */
      if(order_buffer[i].get_value() <= 0) {
        tmp_filter.filter_component(component);
        continue;
      }

      /* Strip component from spectra */
      t_spectra_filter strip_filter = tmp_filter;
      tmp_filter.filter_component(component);
      strip(component, spectra, strip_filter);

      /* Insert the component into the candidate */
      std::pair<t_candidate::iterator, bool> tmp = candidate.insert(component);
      assert(tmp.second);

      calculate(spectra, D, &strip_filter, candidate, start_time);

      candidate.erase(tmp.first);
    }
  }

private:
  bool all_failed(t_component_id component, 
                  const t_spectra <T_ACTIVITY> & spectra,
                  const t_spectra_filter & filter) const {

    t_spectra_iterator it(spectra.get_component_count(),
                          spectra.get_transaction_count(),
                          &filter);

    while (it.next_transaction()){
      t_transaction_id transaction = it.get_transaction();
      if(!spectra.is_error(transaction))
        continue;

      bool activity = spectra.get_count(component, transaction);
      if(!activity)
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
    
    while (it.next_transaction()){
      t_transaction_id transaction = it.get_transaction();
      bool activity = spectra.get_count(component, transaction);
      if(activity)
        filter.filter_transaction(transaction);
    }
    filter.filter_component(component);
  }

  const t_heuristic<T_ACTIVITY> & get_heuristic(t_count level) const {
    typename t_heuristics::const_iterator it = heuristics.upper_bound(level);
    it--;
    return it->second;
  }

};

#endif
