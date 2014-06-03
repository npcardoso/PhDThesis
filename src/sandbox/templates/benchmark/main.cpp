#include "topologies.h"

#include <libdiag/diag.h>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace diagnosis;
using namespace diagnosis::algorithms;
using namespace diagnosis::heuristics;
using namespace diagnosis::benchmark;
using namespace diagnosis::structs;

t_ptr<t_spectra_generator> create_n_tier_generator (t_count n_errors,
                                                    t_count n_faults,
                                                    std::mt19937 & gen) {
    // Generation settings
    t_ptr<t_topology_based_generator> gen_settings(new t_topology_based_generator());
    gen_settings->set_until_nerrors(n_errors);
    gen_settings->set_max_activations(20);

    // Topology Randomizer
    t_n_tier_setup n_tier(gen_settings);
    n_tier.set_fault_type(t_fault(0, 0.9, 0.1, 0));
    n_tier.set_levels(3, 10);
    n_tier.set_per_level(3, 10);
    n_tier.set_n_faults(n_faults, n_faults);

    // Create spectra generators

    return generate_generators(50, 10, n_tier, gen);
}

t_ptr<t_benchmark_settings> create_benchmark_settings (std::string dest) {
    // Metrics
    t_metrics_hook * metrics_hook = new t_metrics_hook();


    metrics_hook->push_back(t_ptr<t_metric> (new t_Cd()));
    metrics_hook->push_back(t_ptr<t_metric> (new t_wasted_effort()));
    metrics_hook->push_back(t_ptr<t_metric> (new t_entropy()));
    metrics_hook->push_back(t_ptr<t_metric> (new t_quality(t_wasted_effort().key())));
    metrics_hook->push_back(t_ptr<t_metric> (new t_quality_fair(t_wasted_effort().key())));

    // Benchmark Hooks
    t_ptr<t_hook_combiner> hook(new t_hook_combiner());

    hook->push_back(t_ptr<t_benchmark_hook> (new t_job_tracker_hook()));
    hook->push_back(t_ptr<t_benchmark_hook> (new t_verbose_hook()));
    hook->push_back(t_ptr<t_benchmark_hook> (new t_save_hook()));
    hook->push_back(t_ptr<t_benchmark_hook> (new t_statistics_hook()));
    hook->push_back(t_ptr<t_benchmark_hook> (metrics_hook));
    hook->push_back(t_ptr<t_benchmark_hook> (new t_flusher_hook()));


    // Collector
    t_const_ptr<t_path_generator> path_generator(new t_path_single_dir(dest));
    t_ptr<t_collector> collector(new t_collector(path_generator));

    // Benchmark
    t_ptr<t_benchmark_settings> settings(
        new t_benchmark_settings(collector, hook));


    // Candidate Generators
    t_ptr<t_mhs> mhs(new t_mhs());
    t_ptr<t_single_fault> single_fault(new t_single_fault());

    mhs->max_time = 3e7;
    settings->add_generator(mhs, "mhs");

    // Candidate Rankers
    t_ptr<t_barinel> barinel(new t_barinel());
    t_ptr<t_barinel> fuzzinel(new t_barinel());
    t_ptr<t_ochiai> ochiai(new t_ochiai());


    barinel->use_confidence = false;
    barinel->use_fuzzy_error = false;
    settings->add_ranker(barinel, "barinel");
    settings->add_ranker(fuzzinel, "fuzzinel");


    settings->add_connection("mhs", "barinel");
    settings->add_connection("mhs", "fuzzinel");

    // settings->add_generator(single_fault, "single_fault");
    // settings->add_ranker(ochiai, "ochiai");
    // benchmark->add_connection("single_fault", "ochiai");

    return settings;
}

t_ptr<t_spectra_generator> create_replay_generator (const t_path_generator & path_generator,
                                                    t_count iterations) {
    t_replay * spectra_generator = new t_replay();
    t_entry entry;


    for (t_count i = 1; i <= iterations; i++) {
        entry[STATUS_KEY_ITERATION] = boost::lexical_cast<std::string> (i);
        string spectra = path_generator(entry, "spectra.txt").string();
        string correct = path_generator(entry, "correct.txt").string();

        t_replay::value_type test_case(spectra, correct);
        spectra_generator->push_back(test_case);
    }

    return t_ptr<t_spectra_generator> (spectra_generator);
}

int main (int argc, char ** argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <folder> <until_nerrors> <n_faults>" << std::endl;
        return 1;
    }

    std::string dest(argv[1]);
    int n_errors = atoi(argv[2]);
    int n_faults = atoi(argv[3]);

    time_t seed = time(NULL);
    std::mt19937 gen(seed);


    // t_ptr<t_spectra_generator> spectra_generator(
    // create_n_tier_generator(n_errors, n_faults, gen));

    t_ptr<t_spectra_generator> spectra_generator(
        create_replay_generator(t_path_single_dir("test"), 500));

    ////Example: How to replay something
    // t_ptr<t_spectra_generator> spectra_generator(
    // create_replay_generator(t_path_single_dir("test"), 20));

    t_ptr<t_benchmark_settings> benchmark_settings(
        create_benchmark_settings(dest));

    // Launch
    run_benchmark(*benchmark_settings,
                  *spectra_generator,
                  gen);


    return 0;
}