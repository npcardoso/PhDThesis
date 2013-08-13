#ifndef __DIAGNOSIS_RANDOMIZER_TOPOLOGY_BASED_H__
#define __DIAGNOSIS_RANDOMIZER_TOPOLOGY_BASED_H__

#include "randomizer.h"

#include "diagnosis/structs/count_spectra.h"
#include "diagnosis/structs/topology.h"

namespace diagnosis {
namespace randomizers {
class t_topology_based : public t_spectra_randomizer<structs::t_count_spectra> {
public:
    DEFINE_BOOST_SHARED_PTRS(t_topology_based);
    t_topology_based (structs::t_topology::t_ptr & topology);

    virtual const t_self_type & operator () (structs::t_count_spectra & spectra,
                                             structs::t_candidate & correct_candidate,
                                             boost::random::mt19937 & gen,
                                             t_transaction_id tran) const;

    virtual const t_self_type & operator () (structs::t_count_spectra & spectra,
                                             structs::t_candidate & correct_candidate,
                                             boost::random::mt19937 & gen) const;

    t_topology_based & add_entry_point (t_component_id comp,
                                        t_probability prob);
    t_topology_based & set_stack_size (t_count size);
    t_topology_based & set_max_transactions (t_count max_transactions);
    t_topology_based & set_until_nerrors (t_count nerrors);

protected:
    t_component_id gen_entry_point (boost::random::mt19937 & gen) const;


private:
    typedef std::pair<t_component_id, t_probability> t_entry_point;


    structs::t_topology::t_ptr & topology;

    std::vector<t_entry_point> entry_points;
    t_count stack_size;
    t_count max_transactions;
    t_count until_nerrors;
};
}
}

#endif