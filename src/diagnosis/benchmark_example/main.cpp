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
    std::string dest("benchmark_results");
    time_t seed = time(NULL);
    mt19937 gen(seed);


    // Spectra Randomizer
    t_topology_based::t_ptr topology_randomizer(new t_topology_based());


    topology_randomizer->set_until_nerrors(2);
    topology_randomizer->set_max_activations(20);

    // Topology Randomizer
    t_forwarding_network::t_ptr fn(new t_forwarding_network());


    fn->fault = t_fault(0, 0.9, 0.1, 0);
    fn->set_levels(3, 10);
    fn->set_per_level(3, 10);
    fn->set_n_faults(2, 6);


    // Spectra Meta Randomizer
    t_topology_based_meta_randomizer meta_randomizer(topology_randomizer, fn);


    // Candidate Generators
    heuristics::t_heuristic heuristic;
    heuristic.push(new heuristics::t_ochiai());
    heuristic.push(new heuristics::t_sort());

    t_mhs * mhs_ptr = new t_mhs(heuristic);
    t_candidate_generator::t_ptr mhs(mhs_ptr);
    t_candidate_generator::t_ptr single_fault(new t_single_fault());

    mhs_ptr->max_time = 1e6;


    // Candidate Rankers
    t_barinel * barinel_ptr = new t_barinel();
    t_candidate_ranker::t_ptr barinel(barinel_ptr);
    t_candidate_ranker::t_ptr fuzzinel(new t_barinel());
    t_candidate_ranker::t_ptr ochiai(new t_ochiai());


    barinel_ptr->use_confidence = false;
    barinel_ptr->use_fuzzy_error = false;

    // Metrics
    t_metrics_hook * metrics_hook = new t_metrics_hook(dest);
    (*metrics_hook) << new t_Cd();
    (*metrics_hook) << new t_wasted_effort();
    (*metrics_hook) << new t_entropy();
    (*metrics_hook) << new t_quality(t_wasted_effort().key());
    (*metrics_hook) << new t_quality_fair(t_wasted_effort().key());

    // Benchmark Hooks
    t_hook_combiner hook;
    hook << new t_verbose_hook() << new t_save_hook(dest) << new t_statistics_hook(dest) << metrics_hook;

    // Benchmark
    t_benchmark benchmark;
    benchmark.add_generator(mhs, "mhs");
    benchmark.add_generator(single_fault, "single_fault");

    benchmark.add_ranker(barinel, "barinel");
    benchmark.add_ranker(fuzzinel, "fuzzinel");
    benchmark.add_ranker(ochiai, "ochiai");

    benchmark.add_connection("mhs", "barinel");
    benchmark.add_connection("mhs", "fuzzinel");
    benchmark.add_connection("single_fault", "ochiai");


    // Launch
    benchmark(meta_randomizer, gen, hook, 1, 1);

    return 0;
}