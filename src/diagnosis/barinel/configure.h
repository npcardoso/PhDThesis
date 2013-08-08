#ifndef __CONFIGURE_MHS_H__
#define __CONFIGURE_MHS_H__

#include "diagnosis/algorithms/mhs.h"
#include "opt.h"
#include "types.h"

#define MPI_BUFFER (1 << 16)

using namespace diagnosis;

class t_mhs_options : public t_options {
public:
    static t_heuristic default_heuristic ();
    t_mhs_options (std::string app_name);
    virtual bool short_opt (int c, char * param);
    virtual std::ostream & print (std::ostream & out) const;

public:
    t_count mpi_level;
    t_count mpi_stride;
    t_count mpi_buffer;

    bool mpi_hierarchical;

    bool fuzzinel;
    bool has_confidence;
    bool ambiguity_groups;

    bool print_spectra;

    algorithms::t_mhs mhs;
};

#endif