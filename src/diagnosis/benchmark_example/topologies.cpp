#include "topologies.h"
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>

namespace diagnosis {
t_topology * t_forwarding_network::operator () (mt19937 & gen) {
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

void generate_topology (mt19937 & gen,
                        t_ptr<t_topology> & topology,
                        const t_fault & fault,
                        t_count ncomp,
                        t_count nfaults,
                        t_count nout,
                        t_count nparallel) {
    t_count c = ncomp;


    uniform_int_distribution<> int_dist(nfaults + nparallel + 1, ncomp);
    uniform_real_distribution<> real_dist(0, 2.0 / ncomp);

    while (c > nfaults + nparallel) {
        topology->add_link(ncomp + 2, t_link().add_sink(c, 0.2).add_sink(0, 0.8));

        t_link tmp_link;

        for (t_count i = 0; i < nout; i++) {
            tmp_link.add_sink(int_dist(gen), real_dist(gen));
        }

        topology->add_link(c, tmp_link);

        c--;
    }

    t_link tmp_link;

    while (c > nfaults) {
        tmp_link.add_sink(c, 1);
        c--;
    }

    while (c) {
        topology->add_fault(c, fault);
        tmp_link.add_sink(c, 1);
        c--;
    }

    topology->add_link(ncomp + 1, tmp_link);
    topology->add_link(ncomp + 1, t_link().add_sink(ncomp + 2, 1));
}
}