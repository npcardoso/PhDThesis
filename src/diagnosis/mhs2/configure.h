#ifndef __CONFIGURE_MHS_H__
#define __CONFIGURE_MHS_H__

#include "algorithms/mhs.h"

#include "utils/opt.h"
#include "types.h"

using namespace diagnosis;


class t_diag_options : public t_options {
public:
    t_diag_options (std::string app_name);
    virtual bool short_opt (int c, char * param);
    virtual std::ostream & print (std::ostream & out) const;

    t_ptr<algorithms::t_mhs> get_mhs () const;
public:
    bool print_spectra;
    bool ambiguity_groups;
    bool conflict_ambiguity;

    typedef enum {NORMAL, PRETTY, LATEX} t_candidate_printer;
    t_candidate_printer candidate_printer;

    t_ptr<algorithms::t_parallelization_factory_random> parallelization;
    t_count threads;


    t_ptr<algorithms::t_similarity> similarity;
    t_ptr<algorithms::t_cutoff> cutoff;
};

#endif