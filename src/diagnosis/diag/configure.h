#ifndef __CONFIGURE_MHS_H__
#define __CONFIGURE_MHS_H__

#include "diagnosis/algorithms/mhs.h"
#include "opt.h"
#include "types.h"

#define MPI_BUFFER (1 << 16)

using namespace diagnosis;
using namespace diagnosis::heuristics;

class t_mhs_options : public t_options {
public:
    static t_heuristic default_heuristic ();
    t_mhs_options (std::string app_name);
    virtual bool short_opt (int c, char * param);
    virtual std::ostream & print (std::ostream & out) const;

public:
    bool has_confidence;
    bool print_spectra;

    t_count mpi_level;
    t_count mpi_stride;
    t_count mpi_buffer;

    bool mpi_hierarchical;

    bool do_ranking;
    bool do_generation;

    bool use_confidence;
    bool use_fuzzy_error;

    bool ambiguity_groups;

    algorithms::t_mhs mhs;
};

#endif