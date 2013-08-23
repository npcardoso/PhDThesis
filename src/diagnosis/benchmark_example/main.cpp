#include "topologies.h"

#include "types.h"
#include "utils/iostream.h"
#include "diagnosis/benchmark/hook_combiner.h"
#include "diagnosis/benchmark/metrics_hook.h"
#include "diagnosis/benchmark/verbose_hook.h"
#include "diagnosis/benchmark/statistics_hook.h"
#include "diagnosis/benchmark/save_hook.h"
#include "diagnosis/benchmark/statistics_hook.h"
#include "diagnosis/benchmark/benchmark.h"
#include "diagnosis/benchmark/metrics.h"
#include "diagnosis/benchmark/benchmark.h"
#include "diagnosis/algorithms/single_fault.h"
#include "diagnosis/algorithms/mhs.h"
#include "diagnosis/algorithms/barinel.h"
#include "diagnosis/heuristics/sort.h"
#include "diagnosis/heuristics/similarity.h"
#include "diagnosis/randomizers/topology_based.h"
#include "diagnosis/structs/count_spectra.h"
#include "diagnosis/structs/trie.h"
#include <list>

using namespace std;
using namespace diagnosis;
using namespace diagnosis::algorithms;
using namespace diagnosis::heuristics;
using namespace diagnosis::benchmark;
using namespace diagnosis::randomizers;
using namespace diagnosis::structs;
#define LEVELS 20
#define PER_LEVEL 10
#define NFAULTS 3

int main (int argc, char ** argv) {
    time_t seed = time(NULL);
    mt19937 gen(seed);
    t_topology::t_ptr topology(new t_topology());


    // generate_topology(gen, topology, , NCOMPS, NFAULTS, 3, 10);
    forwarding_network_topology(gen,
                                topology,
                                t_fault(0, 0.9, 0.1, 0), // fault
                                LEVELS, // n_levels
                                PER_LEVEL, // per_level
                                NFAULTS); // n_faults

    if (argc > 1) {
        topology->graphviz(std::cout);
        return 0;
    }

    t_topology_based topology_randomizer(topology);

    topology_randomizer
    .set_until_nerrors(2)
    .set_max_activations(20)
    .add_entry_point(LEVELS * PER_LEVEL + 2, 0.5);


    heuristics::t_heuristic heuristic;
    heuristic.push(new heuristics::t_ochiai());
    heuristic.push(new heuristics::t_sort());

    t_mhs * mhs_ptr;
    t_candidate_generator::t_ptr mhs(mhs_ptr = new t_mhs(heuristic));
    t_candidate_generator::t_ptr single_fault(new t_single_fault());
    t_barinel * barinel_ptr;
    t_candidate_ranker::t_ptr barinel(barinel_ptr = new t_barinel());
    t_candidate_ranker::t_ptr fuzzinel(new t_barinel());
    t_candidate_ranker::t_ptr ochiai(new t_ochiai());


    mhs_ptr->max_time = 1e6;

    barinel_ptr->use_confidence = false;
    barinel_ptr->use_fuzzy_error = false;

    t_benchmark<t_count_spectra> benchmark;
    benchmark.add_generator(mhs);
    benchmark.add_generator(single_fault);

    benchmark.add_ranker(barinel);
    benchmark.add_ranker(fuzzinel);
    benchmark.add_ranker(ochiai);

    benchmark.add_connection(1, 1);
    benchmark.add_connection(1, 2);
    benchmark.add_connection(2, 3);

    t_hook_combiner hook;
    std::string dest("foooo");
    t_metrics_hook * metrics_hook = new t_metrics_hook(dest);
    (*metrics_hook) << new t_Cd() << new t_wasted_effort() << new t_entropy();
    hook << new t_verbose_hook() << new t_save_hook(dest) << new t_statistics_hook(dest) << metrics_hook;

    for (t_id i = 0; i < 10; i++) {
        t_count_spectra spectra;
        t_candidate correct;

        benchmark(topology_randomizer, gen, hook);
        // BOOST_FOREACH(t_benchmark_result & r, res) {
        // std::cout << "Setup G:" << r.generator << " R:" << r.ranker << " Cd: " << r.cost << " | ";
        // }
        std::cout << std::endl;
    }


    return 0;
}