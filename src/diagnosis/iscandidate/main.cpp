#include <libdiag/structs/count_spectra.h>
#include <libdiag/types.h>
#include "configure.h"

using namespace diagnosis;
using namespace diagnosis::structs;

int main (int argc, char ** argv) {
    t_count_spectra spectra;
    t_iscandidate_options options(argv[0]);


    if (options.configure(argc, argv))
        return 1;


    options.spectra_input() >> spectra;

    while (true) {
        t_candidate c;
        options.input() >> c;

        if (!c.size())
            break;

        options.output() << spectra.is_candidate(c) << std::endl;
    }

    return 0;
}