#ifndef __MHS_H__
#define __MHS_H__

#include "../heuristic/heuristic.h"
#include "../spectra/spectra.h"
#include "../spectra/spectra_filter.h"
#include "../spectra/spectra_iterator.h"
#include "../common/trie.h"
#include "../common/utils.h"

#include <map>
#include <list>

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
  
  const t_heuristic<T_ACTIVITY> & get_heuristic(t_count level) const {
    typename t_heuristics::const_iterator it = heuristics.upper_bound(level);
    it--;
    return it->second;
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
                 t_candidate & candidate,
                 t_time_interval start_time=get_time_interval()) const {

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
          /* Result Length cutoff */

          if(max_candidates && D.size() >= max_candidates)
            return;

          std::pair<t_candidate::iterator, bool> tmp = candidate.insert(it.get_component());
          assert(tmp.second);

          D.add(candidate);
          candidate.erase(tmp.first);

          if(max_candidates && D.size() >= max_candidates)
            return;

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
      if(max_candidates && D.size() >= max_candidates)
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

  void update(const t_spectra <T_ACTIVITY> & spectra,
              t_trie & D,
              t_trie & old_D,
              const t_spectra_filter & filter) const {

    std::list <t_candidate> candidates;

    {
      t_trie::iterator it = old_D.begin();
      while(it != old_D.end()) {
        if(spectra.is_candidate(*it, &filter))
          D.add(*it, true);
        else
          candidates.push_back(*it);
        it++;
      }
    }
    {
      std::list<t_candidate>::iterator it = candidates.begin();
      while(it != candidates.end()){
        t_candidate candidate = *it;
        t_spectra_filter tmp_filter = filter;
        strip(candidate, spectra, tmp_filter);
        calculate(spectra, D, &tmp_filter, candidate);
        it++;
      }
    }
  }

  static void combine(const t_spectra <T_ACTIVITY> & spectra,
                      t_trie & D,
                      const t_trie & D_first,
                      const t_trie & D_second,
                      const t_spectra_filter & filter_first,
                      const t_spectra_filter & filter_second) {

    std::list <t_candidate> c_first, c_second;

    {
      t_trie::iterator it = D_first.begin();
      while(it != D_first.end()) {
        if(spectra.is_candidate(*it, &filter_second))
          D.add(*it);
        else if(!D_second.is_composite(*it, true))
          c_first.push_back(*it);
        it++;
      }

      it = D_second.begin();
      while(it != D_second.end()) {
        if(spectra.is_candidate(*it, &filter_first))
          D.add(*it);
        else if(!D_first.is_composite(*it, true))
          c_second.push_back(*it);
        it++;
      }
    }
    {
      std::list<t_candidate>::iterator first_it = c_first.begin();
      while(first_it != c_first.end()){
        std::list<t_candidate>::iterator second_it = c_second.begin();
        while(second_it != c_second.end()){
          t_candidate tmp = *first_it;
          tmp.insert(second_it->begin(), second_it->end());
          D.add(tmp);
          second_it++;
        }
        first_it++;
      }
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

  void strip(t_candidate candidate,
             const t_spectra <T_ACTIVITY> & spectra,
             t_spectra_filter & filter) const {

    t_spectra_iterator it(spectra.get_component_count(),
                          spectra.get_transaction_count(),
                          &filter);

    while (it.next_transaction()) {
      t_candidate::const_iterator c_it = candidate.begin();
      while (c_it != candidate.end()) {
        t_transaction_id transaction = it.get_transaction();
        if(spectra.get_count(*(c_it++), transaction)) {
          filter.filter_transaction(transaction);
          break;
        }
      }
    }

    t_candidate::const_iterator c_it = candidate.begin();
    while (c_it != candidate.end())
      filter.filter_component(*(c_it++));
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


};

#endif
