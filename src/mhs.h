#ifndef __MHS_H__
#define __MHS_H__

#include "heuristic.h"
#include "count_spectra.h"
#include "spectra.h"
#include "spectra_filter.h"
#include "spectra_iterator.h"
#include "trie.h"
#include "utils.h"

#include <boost/shared_ptr.hpp>
#include <map>

template <class T_ACTIVITY>
class t_mhs {
  typedef boost::shared_ptr<t_heuristic<T_ACTIVITY> > t_heuristic_ptr;
  typedef std::map<t_count, t_heuristic_ptr> t_heuristics;

  t_heuristics heuristics;

public:
  t_count max_candidate_size, max_candidates;
  
  t_mhs(t_heuristic<T_ACTIVITY> * heuristic) {
    max_candidate_size = 0;
    set_heuristic(0, heuristic);
  }

  void set_heuristic(t_count start_level, t_heuristic<T_ACTIVITY> * heuristic) {
    assert(heuristic != NULL);
    heuristics[start_level] = t_heuristic_ptr(heuristic);
  }
  
  void calculate(const t_spectra <T_ACTIVITY> & spectra,
                 t_trie & D,
                 const t_spectra_filter * filter = NULL) const {

    t_candidate candidate;
    calculate(spectra, D, filter, candidate);
  
  }
  
  void calculate(const t_spectra <T_ACTIVITY> & spectra,
                 t_trie & D,
                 const t_spectra_filter * filter,
                 t_candidate & candidate) const {
    
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

          
          D.add(candidate);
          candidate.erase(tmp.first);
          
          /* Result Length cutoff */
          if(D.size() >= max_candidates)
            return;

          tmp_filter.filter_component(it.get_component());
        }
    }

    /* Candidate Length cutoff */

    if(max_candidate_size && candidate.size() + 2 > max_candidate_size)
      return;

    /* Ranking */

    t_order_buffer order_buffer = spectra.get_ordering_buffer(&tmp_filter);

    get_heuristic(candidate.size()).order(spectra, order_buffer.get(), &tmp_filter);

    /* Creating complex candidates */

    t_count remaining_components = spectra.get_component_count() - tmp_filter.get_filtered_component_count();

    for(t_id i = 0; i < remaining_components; i++) {
      t_component_id component = order_buffer[i].get_component();

      /* Heuristic signalling end of ranking */
      if(component == 0)
        break;

      /* Strip component from spectra */
      
      t_spectra_filter strip_filter = tmp_filter;
      
      tmp_filter.filter_component(component);

      /* Heuristic signalling skip */
      if(order_buffer[i].get_value() <= 0)
        continue;

      strip(component, spectra, strip_filter);
      
      /* Insert the component into the candidate */
      
      std::pair<t_candidate::iterator, bool> tmp = candidate.insert(component);
      assert(tmp.second);

      calculate(spectra, D, &strip_filter, candidate);
      
      candidate.erase(tmp.first);
      
      /* Result Length cutoff */
      if(D.size() >= max_candidates)
        return;
    }
  }

private:
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

  const t_heuristic<T_ACTIVITY> & get_heuristic(t_count level) const {
    typename t_heuristics::const_iterator it = heuristics.upper_bound(level);
    it--;
    return *(it->second);
  }

};

#endif
