#ifndef __TOPOLOGIES_H_5297dae76cee7fb6b71d29e25f33dca1337a8ffd__
#define __TOPOLOGIES_H_5297dae76cee7fb6b71d29e25f33dca1337a8ffd__

#include <libdiag/benchmark/generators/topology_based.h>
#include <libdiag/structs/spectra.h>
#include <libdiag/structs/topology.h>

using namespace boost::random;
using namespace diagnosis::structs;
using namespace diagnosis::benchmark;

class t_topology_generator_generator {
public:
    inline t_topology_generator_generator (t_ptr<t_topology_based_generator> settings) : settings(settings) {}

    virtual t_spectra_generator * operator () (std::mt19937 & gen) {
        t_topology_based_generator * tmp = new t_topology_based_generator(*settings);


        tmp->set_topology(generate_topology(gen));
        return tmp;
    }

protected:
    virtual t_topology * generate_topology (std::mt19937 & gen) = 0;

private:
    t_ptr<t_topology_based_generator> settings;
};


class t_n_tier_setup : public t_topology_generator_generator {
public:
    using t_topology_generator_generator::t_topology_generator_generator;

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

    inline void set_fault_type (const t_fault & fault) {
        this->fault = fault;
    }

protected:
    virtual t_topology * generate_topology (std::mt19937 & gen);

private:
    t_fault fault;
    t_count levels_min, levels_max;
    t_count per_level_min, per_level_max;
    t_count n_faults_min, n_faults_max;
    diagnosis::t_probability sink_prob_min, sink_prob_max;
};


#endif