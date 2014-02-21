#include "topologies.h"
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>

t_topology * t_n_tier_setup::generate_topology (std::mt19937 & gen) {
    t_topology * topology = new t_topology();

    boost::random::uniform_int_distribution<> per_level_dist(per_level_min, per_level_max);

    t_count levels = uniform_int_distribution<> (levels_min, levels_max) (gen);

    t_count next_level_count = per_level_dist(gen);
    t_count cur_level_count = 1;
    t_count base = 1;

    {
        t_link link;

        for (t_id c_dst = 0; c_dst < next_level_count; c_dst++)
            link.add_sink(c_dst + base + cur_level_count, 1);

        topology->add_link(base, link);
    }


    for (t_id level = 1; level < levels; level++) {
        t_link link;

        base += cur_level_count;
        cur_level_count = next_level_count;
        next_level_count = per_level_dist(gen);

        for (t_id c_dst = 0; c_dst < next_level_count; c_dst++)
            link.add_sink(base + cur_level_count + c_dst, 1);

        for (t_id c_src = 0; c_src < cur_level_count; c_src++)
            topology->add_link(base + c_src, link);
    }

    base += cur_level_count + next_level_count;

    t_count n_faults = uniform_int_distribution<> (n_faults_min, n_faults_max) (gen);

    while (n_faults) {
        t_component_id c = uniform_int_distribution<> (1, base - 1) (gen);

        // If the component wasn't already faulty
        if (!topology->get_fault(c)) {
            topology->add_fault(c, fault);
            n_faults--;
        }
    }

    // Entry point
    topology->add_entry_point(1, 1);
    return topology;
}