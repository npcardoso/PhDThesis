#include "configure.h"
#include "structs/ambiguity_groups.h"
#include "structs/count_spectra.h"
#include "structs/diagnosis_report.h"
#include "utils/iostream.h"


#include <list>

using namespace diagnosis;
using namespace diagnosis::structs;

void do_stuff (const t_spectra & spectra, t_diag_options & options) {
    t_ambiguity_groups ambiguity_groups;


    if (options.print_spectra)
        spectra.print(options.output());

    // Check for invalid transactions
    t_spectra::t_invalid_transactions invalid_transactions;

    if (spectra.get_invalid(invalid_transactions)) {
        std::cerr << "Invalid spectra (some failing transactions do not activate any components: " << invalid_transactions << ")" << std::endl;
        return;
    }

    // Check ambiguity groups
    if (options.ambiguity_groups) {
        ambiguity_groups = t_ambiguity_groups(spectra);

        if (options.print_spectra)
            spectra.write(options.output(), &ambiguity_groups.filter()) << std::endl;

        options.output() << ambiguity_groups << std::endl;
    }

    if (options.do_generation) {
        t_trie D;
        options.mhs(spectra, D); // TODO: paralellization

        if (options.do_ranking) {
            // Fuzzinel stuff
            t_candidate_ranker::t_ret_type probs;
            options.barinel(spectra, D, probs);

            t_diagnosis_report report(D, probs);
            options.output() << report;
        }
        else {
            options.output() << D << "0 0\n";
        }
    }
}

int main (int argc, char ** argv) {
    t_diag_options options(argv[0]);


    if (options.configure(argc, argv))
        return 1;

    options.debug() << options << std::endl;

    while (options.input().good()) {
        t_count_spectra spectra;
        // Spectra IO
        spectra.read(options.input(), options.has_confidence);

        if (options.input().fail() || options.input().bad()) {
            std::cerr << "Problem reading spectra" << std::endl;
            return 1;
        }

        do_stuff(spectra, options);
    }

    return 0;
}