#include "topologies.h"
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>

void forwarding_network_topology (mt19937 & gen,
                                  t_topology::t_ptr & topology,
                                  const t_fault & fault,
                                  t_count levels,
                                  t_count per_level,
                                  t_count n_faults) {
    {
        t_link link;

        for (t_id c_dst = 0; c_dst < per_level; c_dst++)
            link.add_sink(c_dst + 1, 1);

        (* topology)(levels * per_level + 2, link);
    }

    for (t_id level = 1; level < levels; level++) {
        t_link link;

        for (t_id c_dst = 0; c_dst < per_level; c_dst++)
            link.add_sink(level * per_level + c_dst + 1, 1);

        for (t_id c_src = 0; c_src < per_level; c_src++)
            (* topology)((level - 1) * per_level + c_src + 1, link);
    }

    while (n_faults) {
        t_component_id c = uniform_int_distribution<> (1, levels * per_level) (gen);

        // If the component wasn't already faulty
        if (!topology->get_fault(c)) {
            (* topology)(c, fault);
            n_faults--;
        }
    }
}

void generate_topology (mt19937 & gen,
                        t_topology::t_ptr & topology,
                        const t_fault & fault,
                        t_count ncomp,
                        t_count nfaults,
                        t_count nout,
                        t_count nparallel) {
    t_count c = ncomp;


    uniform_int_distribution<> int_dist(nfaults + nparallel + 1, ncomp);
    uniform_real_distribution<> real_dist(0, 2.0 / ncomp);

    while (c > nfaults + nparallel) {
        (* topology)(ncomp + 2, t_link().add_sink(c, 0.2).add_sink(0, 0.8));

        t_link tmp_link;

        for (t_count i = 0; i < nout; i++) {
            tmp_link.add_sink(int_dist(gen), real_dist(gen));
        }

        (* topology)(c, tmp_link);

        c--;
    }

    t_link tmp_link;

    while (c > nfaults) {
        tmp_link.add_sink(c, 1);
        c--;
    }

    while (c) {
        (* topology)(c, fault);
        tmp_link.add_sink(c, 1);
        c--;
    }

    (* topology) (ncomp + 1, tmp_link) (ncomp + 1, t_link().add_sink(ncomp + 2, 1));
}