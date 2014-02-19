#include "configure.h"
#include "diagnosis/structs/ambiguity_groups.h"
#include "diagnosis/structs/count_spectra.h"
#include "diagnosis/structs/diagnosis_report.h"
#include "opt.h"
#include "utils/time.h"
#include "utils/iostream.h"


#include <list>

using namespace diagnosis;
using namespace diagnosis::structs;


int main (int argc, char ** argv) {
    t_diag_options options(argv[0]);
    t_ambiguity_groups ambiguity_groups;


    if (options.configure(argc, argv))
        return 1;

    t_count_spectra spectra;
    structs::t_trie D;
    algorithms::t_mhs mhs(options.mhs);

    options.debug() << options << std::endl;


    // Spectra IO
    spectra.read(options.input(), options.has_confidence);

    if (options.input().fail() || options.input().bad()) {
        std::cerr << "Problem reading spectra" << std::endl;
        return 1;
    }

    if (options.print_spectra)
        spectra.print(options.output());

    // Check for invalid transactions
    t_spectra::t_invalid_transactions invalid_transactions;

    if (spectra.get_invalid(invalid_transactions)) {
        std::cerr << "Invalid spectra (some failing transactions do not activate any components: " << invalid_transactions << ")" << std::endl;
        return 1;
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
            options.output() << D;
        }
    }

    return 0;
}