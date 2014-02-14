#include "topologies.h"

#include "types.h"
#include "utils/iostream.h"
#include "diagnosis/benchmark/path_generator.h"
#include "diagnosis/benchmark/hooks/hook_combiner.h"
#include "diagnosis/benchmark/hooks/metrics_hook.h"
#include "diagnosis/benchmark/hooks/verbose_hook.h"
#include "diagnosis/benchmark/hooks/statistics_hook.h"
#include "diagnosis/benchmark/hooks/save_hook.h"
#include "diagnosis/benchmark/hooks/statistics_hook.h"
#include "diagnosis/benchmark/metrics.h"
#include "diagnosis/benchmark/benchmark.h"
#include "diagnosis/benchmark/benchmark_settings.h"
#include "diagnosis/algorithms/single_fault.h"
#include "diagnosis/algorithms/mhs.h"
#include "diagnosis/algorithms/barinel.h"
#include "diagnosis/heuristics/sort.h"
#include "diagnosis/heuristics/similarity.h"
#include "diagnosis/randomizers/repeater.h"
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
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <folder> <until_nerrors> <n_faults>" << std::endl;
        return 1;
    }

    std::string dest(argv[1]);
    int n_errors = atoi(argv[2]);
    int n_faults = atoi(argv[3]);

    time_t seed = time(NULL);
    boost::mt19937 gen(seed);


    // Spectra Randomizer
    t_topology_based::t_ptr topology_randomizer(new t_topology_based());


    topology_randomizer->set_until_nerrors(n_errors);
    topology_randomizer->set_max_activations(20);

    // Topology Randomizer
    t_forwarding_network::t_ptr fn(new t_forwarding_network());


    fn->fault = t_fault(0, 0.9, 0.1, 0);
    fn->set_levels(3, 10);
    fn->set_per_level(3, 10);
    fn->set_n_faults(n_faults, n_faults);


    // Architecture
    t_architecture::t_ptr architecture(new t_topology_based_architecture(topology_randomizer, fn));
    architecture = t_architecture::t_ptr(new t_architecture_repeater(architecture, 1, 10));

    // Candidate Generators
    heuristics::t_heuristic heuristic;
    heuristic.push(new heuristics::t_ochiai());
    heuristic.push(new heuristics::t_sort());

    t_mhs * mhs_ptr = new t_mhs(heuristic);
    t_candidate_generator::t_const_ptr mhs(mhs_ptr);
    t_candidate_generator::t_const_ptr single_fault(new t_single_fault());

    mhs_ptr->max_time = 1e6;


    // Candidate Rankers
    t_barinel * barinel_ptr = new t_barinel();
    t_candidate_ranker::t_const_ptr barinel(barinel_ptr);
    t_candidate_ranker::t_const_ptr fuzzinel(new t_barinel());
    t_candidate_ranker::t_const_ptr ochiai(new t_ochiai());


    barinel_ptr->use_confidence = false;
    barinel_ptr->use_fuzzy_error = false;

    // Metrics
    t_metrics_hook * metrics_hook = new t_metrics_hook();
    (*metrics_hook) << new t_Cd();
    (*metrics_hook) << new t_wasted_effort();
    (*metrics_hook) << new t_entropy();
    (*metrics_hook) << new t_quality(t_wasted_effort().key());
    (*metrics_hook) << new t_quality_fair(t_wasted_effort().key());

    // Benchmark Hooks
    t_hook_combiner * hook_ptr = new t_hook_combiner();
    (*hook_ptr) << new t_verbose_hook();
    // hook_ptr << new t_save_hook(dest);
    (*hook_ptr) << new t_statistics_hook();
    (*hook_ptr) << metrics_hook;


    t_benchmark_hook::t_const_ptr hook(hook_ptr);


    // Collector

    t_path_generator::t_const_ptr path_generator(new t_path_single_dir(dest));
    t_collector::t_ptr collector(new t_collector(path_generator));

    // Job Queue

    t_ptr<t_job_queue> job_queue(new t_job_queue());

    // Benchmark
    t_benchmark_settings settings(collector, hook, job_queue);

    settings.add_generator(mhs, "mhs");
    // settings.add_generator(single_fault, "single_fault");

    settings.add_ranker(barinel, "barinel");
    settings.add_ranker(fuzzinel, "fuzzinel");
    // settings.add_ranker(ochiai, "ochiai");

    settings.add_connection("mhs", "barinel");
    settings.add_connection("mhs", "fuzzinel");
    // benchmark.add_connection("single_fault", "ochiai");


    // Execution Controller
    t_execution_controller controller(3);

    // Launch
    run_benchmark(*architecture,
                  gen,
                  settings,
                  controller);


    return 0;
}