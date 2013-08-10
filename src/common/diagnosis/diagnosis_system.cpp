#include "diagnosis_system.h"
namespace diagnosis {
void t_diagnosis_system::operator () (const structs::t_spectra & spectra,
                                      t_candidate_generator::t_ret_type & D,
                                      t_candidate_ranker::t_ret_type & probs,
                                      const structs::t_spectra_filter * filter) const {
    (* generator)(spectra, D, filter);
    (* ranker)(spectra, D, probs, filter);
}
}