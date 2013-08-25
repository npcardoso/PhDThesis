#ifndef __TOPOLOGY_BASED_H_8e80158d1ddf72934776ef3d6638563f62a4dc93__
#define __TOPOLOGY_BASED_H_8e80158d1ddf72934776ef3d6638563f62a4dc93__

#include "randomizer.h"

#include "diagnosis/structs/count_spectra.h"
#include "diagnosis/structs/topology.h"

namespace diagnosis {
namespace randomizers {
class t_topology_based : public t_spectra_randomizer {
public:
    DEFINE_BOOST_SHARED_PTRS(t_topology_based);
    t_topology_based ();
    t_topology_based (structs::t_topology::t_ptr & topology);

    virtual structs::t_spectra * operator () (boost::random::mt19937 & gen,
                                              structs::t_candidate & correct_candidate) const;

    t_topology_based & set_topology (structs::t_topology::t_ptr & topology);
    t_topology_based & set_topology (structs::t_topology * topology);

    t_topology_based & set_stack_size (t_count size);
    t_topology_based & set_max_transactions (t_count max_transactions);
    t_topology_based & set_max_activations (t_count max_activations);
    t_topology_based & set_until_nerrors (t_count nerrors);

    virtual std::ostream & write (std::ostream & out) const;

protected:
    virtual const t_topology_based & operator () (structs::t_count_spectra & spectra,
                                                  structs::t_candidate & correct_candidate,
                                                  boost::random::mt19937 & gen,
                                                  t_transaction_id tran) const;


private:


    structs::t_topology::t_ptr topology;

    t_count stack_size;
    t_count max_activations;
    t_count max_transactions;
    t_count until_nerrors;
};
}
}

#endif