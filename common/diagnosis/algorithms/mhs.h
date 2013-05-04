#ifndef __DIAGNOSIS_ALGORITHMS_MHS_H__
#define __DIAGNOSIS_ALGORITHMS_MHS_H__

#include "diagnosis/heuristic.h"
#include "diagnosis/spectra.h"
#include "diagnosis/spectra_filter.h"
#include "diagnosis/spectra_iterator.h"
#include "diagnosis/structs/trie.h"
#include "utils/time.h"

#include <map>

namespace diagnosis {
namespace algorithms {

class t_mhs {
  typedef std::map<t_count, t_heuristic> t_heuristics;

  t_heuristics heuristics;

public:
  t_count max_candidate_size, max_candidates;
  t_time_interval max_time;

  t_mhs(const t_heuristic & heuristic);

  void set_heuristic(t_count start_level, const t_heuristic & heuristic);
  const t_heuristic & get_heuristic(t_count level) const;

  void calculate(const t_spectra & spectra,
                 structs::t_trie & D,
                 const t_spectra_filter * filter = NULL) const;

  void calculate(const t_spectra & spectra,
                 structs::t_trie & D,
                 const t_spectra_filter * filter,
                 t_candidate & candidate,
                 t_time_interval start_time=time_interval()) const;

  void update(const t_spectra & spectra,
              structs::t_trie & D,
              structs::t_trie & old_D,
              const t_spectra_filter & filter) const;


  static void combine(const t_spectra & spectra,
                      structs::t_trie & D,
                      const structs::t_trie & D_first,
                      const structs::t_trie & D_second,
                      const t_spectra_filter & filter_first,
                      const t_spectra_filter & filter_second);

private:
  bool all_failed(t_component_id component, 
                  const t_spectra & spectra,
                  const t_spectra_filter & filter) const;

  void strip(t_candidate candidate,
             const t_spectra & spectra,
             t_spectra_filter & filter) const;


  void strip(t_component_id component,
             const t_spectra & spectra,
             t_spectra_filter & filter) const;



};

}
}

#endif
