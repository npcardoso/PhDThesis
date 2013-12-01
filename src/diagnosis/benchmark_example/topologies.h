#ifndef __TOPOLOGIES_H_5297dae76cee7fb6b71d29e25f33dca1337a8ffd__
#define __TOPOLOGIES_H_5297dae76cee7fb6b71d29e25f33dca1337a8ffd__

#include <boost/random/mersenne_twister.hpp>
#include "diagnosis/randomizers/randomizer.h"
#include "diagnosis/randomizers/topology_based.h"
#include "diagnosis/structs/spectra.h"
#include "diagnosis/structs/topology.h"

using namespace boost::random;
using namespace diagnosis::randomizers;
using namespace diagnosis::structs;

namespace diagnosis {
class t_forwarding_network : public t_randomizer<t_topology> {
public:
    DEFINE_BOOST_SHARED_PTRS(t_forwarding_network);
    virtual t_topology * operator () (boost::random::mt19937 & gen);

    inline void set_levels (t_count min, t_count max) {
        levels_min = min;
        levels_max = max;
    }

    inline void set_per_level (t_count min, t_count max) {
        per_level_min = min;
        per_level_max = max;
    }

    inline void set_n_faults (t_count min, t_count max) {
        n_faults_min = min;
        n_faults_max = max;
    }

public:
    t_fault fault;
    t_count levels_min, levels_max;
    t_count per_level_min, per_level_max;
    t_count n_faults_min, n_faults_max;
    diagnosis::t_probability sink_prob_min, sink_prob_max;
};

class t_topology_based_meta_randomizer : public t_architecture {
public:
    inline t_topology_based_meta_randomizer (t_topology_based::t_ptr settings,
                                             t_randomizer<t_topology>::t_ptr topology_randomizer) : settings(settings), topology_randomizer(topology_randomizer) {}

    virtual t_system * operator () (boost::random::mt19937 & gen) const {
        t_topology_based * tmp = new t_topology_based(*settings);


        tmp->set_topology((* topology_randomizer)(gen));
        return tmp;
    }

private:
    t_topology_based::t_ptr settings;
    t_randomizer<t_topology>::t_ptr topology_randomizer;
};
}
#endif