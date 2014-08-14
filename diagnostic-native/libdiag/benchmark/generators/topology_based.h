#ifndef __TOPOLOGY_BASED_H_8e80158d1ddf72934776ef3d6638563f62a4dc93__
#define __TOPOLOGY_BASED_H_8e80158d1ddf72934776ef3d6638563f62a4dc93__

#include "generator.h"

#include "../../structs/count_spectra.h"
#include "../../structs/topology.h"

namespace diagnostic {
namespace benchmark {
class t_topology_based_generator : public t_spectra_generator {
public:
    t_topology_based_generator ();
    t_topology_based_generator (const t_const_ptr<structs::t_topology> & topology);

    virtual structs::t_spectra * operator () (std::mt19937 & gen,
                                              structs::t_candidate & correct_candidate);

    void set_topology (const structs::t_topology * topology);
    void set_topology (const t_const_ptr<structs::t_topology> & topology);

    void set_stack_size (t_count size);
    void set_max_transactions (t_count max_transactions);
    void set_max_activations (t_count max_activations);
    void set_until_nerrors (t_count nerrors);

    virtual std::ostream & write (std::ostream & out) const;

protected:
    void operator () (structs::t_count_spectra & spectra,
                      structs::t_candidate & correct_candidate,
                      std::mt19937 & gen,
                      t_transaction_id tran) const;
private:
    t_const_ptr<structs::t_topology> topology;

    t_count stack_size;
    t_count max_activations;
    t_count max_transactions;
    t_count until_nerrors;
};
}
}

#endif
