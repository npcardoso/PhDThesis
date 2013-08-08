#include "diagnosis/structs/count_spectra.h"
#include "diagnosis/algorithms/cmeans.h"
#include "configure.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace diagnosis;
using namespace diagnosis::structs;

int main (int argc, char ** argv) {
    t_cmeans_options options(argv[0]);


    if (options.configure(argc, argv))
        return 1;

    t_count_spectra spectra;
    options.input() >> spectra;

    algorithms::t_cmeans cm(spectra);
    t_membership fuzzy_cluster = cm.clustering(options.configs);

    options.output() << fuzzy_cluster;

    return 0;
}