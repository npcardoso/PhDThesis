#ifndef __MHS_H_957e289b8923d6c40fa69c8aa7a53a6ed1da023b__
#define __MHS_H_957e289b8923d6c40fa69c8aa7a53a6ed1da023b__

#include "diagnosis/heuristics/heuristic.h"
#include "diagnosis/diagnosis_system.h"
#include "diagnosis/structs/trie.h"
#include "utils/time.h"

#include <map>

namespace diagnosis {
namespace algorithms {
class t_mhs : public t_candidate_generator {
public:
    DEFINE_BOOST_SHARED_PTRS(t_mhs);

    t_count max_candidate_size, max_candidates;
    t_time_interval max_time;

    t_mhs (const heuristics::t_heuristic & heuristic);

    void set_heuristic (t_count start_level, const heuristics::t_heuristic & heuristic);
    const heuristics::t_heuristic & get_heuristic (t_count level) const;

    virtual void operator () (const structs::t_spectra & spectra,
                              t_ret_type & D,
                              const structs::t_spectra_filter * filter=NULL) const;

    void calculate (const structs::t_spectra & spectra,
                    structs::t_trie & D,
                    const structs::t_spectra_filter * filter,
                    structs::t_candidate & candidate,
                    t_time_interval start_time=time_interval()) const;

    void update (const structs::t_spectra & spectra,
                 structs::t_trie & D,
                 structs::t_trie & old_D,
                 const structs::t_spectra_filter & filter) const;


    static void combine (const structs::t_spectra & spectra,
                         structs::t_trie & D,
                         const structs::t_trie & D_first,
                         const structs::t_trie & D_second,
                         const structs::t_spectra_filter & filter_first,
                         const structs::t_spectra_filter & filter_second);

private:
    bool all_failed (t_component_id component,
                     const structs::t_spectra & spectra,
                     const structs::t_spectra_filter & filter) const;

    void strip (structs::t_candidate candidate,
                const structs::t_spectra & spectra,
                structs::t_spectra_filter & filter) const;


    void strip (t_component_id component,
                const structs::t_spectra & spectra,
                structs::t_spectra_filter & filter) const;
private:
    typedef std::map<t_count, heuristics::t_heuristic> t_heuristics;

    t_heuristics heuristics;
};
}
}

#endif