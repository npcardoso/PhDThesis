#include "types.h"
#include "utils/iostream.h"
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
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <list>

using namespace std;
using namespace diagnosis;
using namespace diagnosis::algorithms;
using namespace diagnosis::heuristics;
using namespace diagnosis::benchmark;
using namespace diagnosis::randomizers;
using namespace diagnosis::structs;
using namespace boost::random;

typedef diagnosis::t_rank_element<const t_candidate *, t_probability_mp> t_rank_element_tmp;
typedef std::vector<t_rank_element_tmp> t_rank;

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

int main (int argc, char ** argv) {
    time_t seed = time(NULL);
    mt19937 gen(seed);
    t_topology::t_ptr topology(new t_topology());


#define NCOMPS 30
#define NFAULTS 10
    generate_topology(gen, topology, t_fault(0, 0, 1, 1), NCOMPS, NFAULTS, 3, 10);

    topology->graphviz(std::cout);

    t_topology_based topology_randomizer(topology);

    topology_randomizer
    .set_until_nerrors(1)
    .set_max_activations(15)
    .add_entry_point(NCOMPS + 1, 0.5);


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

    for (t_id i = 0; i < 10; i++) {
        t_count_spectra spectra;
        t_candidate correct;
        t_save_hook<t_count_spectra> hook("fooo");

        benchmark(topology_randomizer, gen, hook);
        // BOOST_FOREACH(t_benchmark_result & r, res) {
        // std::cout << "Setup G:" << r.generator << " R:" << r.ranker << " Cd: " << r.cost << " | ";
        // }
        std::cout << std::endl;
    }


    return 0;
}