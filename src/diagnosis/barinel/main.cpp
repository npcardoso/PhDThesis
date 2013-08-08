#include "configure.h"
#include "diagnosis/spectra/count_spectra.h"
#include "diagnosis/algorithms/barinel.h"
#include "mpi.h"
#include "opt.h"
#include "utils/time.h"


#include <list>
#include <mpi.h>

using namespace diagnosis;


int main (int argc, char ** argv) {
    t_mhs_options options(argv[0]);


    if (options.configure(argc, argv))
        return 1;

    t_count_spectra spectra;
    structs::t_trie D;
    algorithms::t_mhs mhs(options.mhs);

    /* Initialize MPI */
    MPI_Init(NULL, NULL);

    /* Find out my identity in the default communicator */
    int ntasks, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    t_stats stats;

    if (rank == 0)
        options.debug() << options << std::endl;

    options.input() >> spectra;

    if (!options.input().good()) {
        std::cerr << "Problem reading spectra" << std::endl;
        return 1;
    }

    if (options.print_spectra)
        options.output() << spectra;

    if (ntasks > 1)
        mhs2_heuristic_setup(mhs, options.mpi_level, options.mpi_stride);

    mhs2_map(mhs, spectra, D, stats);

    if (ntasks > 1)
        mhs2_reduce(D, options.mpi_hierarchical, options.mpi_buffer, stats);

    mhs2_collect_stats(options.debug(), D, stats);

    if (rank == 0) {
        if (options.fuzzinel) {
            // Fuzzinel stuff
            diagnosis::algorithms::t_barinel barinel;
            diagnosis::t_probability_mp ret;
            diagnosis::t_probability_mp total_ret(0);
            std::vector<std::pair<diagnosis::t_goodness_mp, t_candidate> > probs;

            diagnosis::structs::t_trie::iterator it = D.begin();

            while (it != D.end()) {
                barinel.calculate(spectra, *it, ret);
                probs.push_back(std::pair<diagnosis::t_goodness_mp, t_candidate> (-ret, *it));
                total_ret += ret;
                it++;
            }

            sort(probs.begin(), probs.end());

            std::vector<std::pair<diagnosis::t_goodness_mp, t_candidate> >::iterator it_prob = probs.begin();

            while (it_prob != probs.end()) {
                options.output() << (-it_prob->first / total_ret) << ": " << it_prob->second << std::endl;
                it_prob++;
            }
        }
        else {
            options.output() << D;
        }
    }

    /* Shut down MPI */
    MPI_Finalize();

    return 0;
}