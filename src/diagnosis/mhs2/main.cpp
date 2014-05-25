#include "configure.h"
#include "structs/ambiguity_groups.h"
#include "structs/count_spectra.h"
#include "utils/iostream.h"

using namespace diagnosis;
using namespace diagnosis::structs;

bool do_stuff (const t_spectra & spectra, t_diag_options & options) {
    t_ambiguity_groups ambiguity_groups;
    t_spectra_filter f;

    // Check for invalid transactions
    t_spectra::t_invalid_transactions invalid_transactions;


    if (spectra.get_invalid(invalid_transactions)) {
        spectra.print(std::cerr);
        std::cerr << "Invalid spectra (some failing transactions do not activate any components: " << invalid_transactions << ")" << std::endl;
        return false;
    }

    // Use only minimal conflicts
    if (options.conflict_ambiguity)
        spectra.get_minimal_conflicts(f);

    // Check ambiguity groups
    if (options.ambiguity_groups)
        ambiguity_groups = t_ambiguity_groups(spectra);

    f.components.filter(ambiguity_groups.filter().components);
    f.transactions.filter(ambiguity_groups.filter().transactions);

    if (options.print_spectra)
        spectra.print(options.output(), &f);

    if (options.ambiguity_groups)
        options.output() << ambiguity_groups << std::endl;


    t_trie D;
    t_ptr<algorithms::t_mhs> mhs = options.get_mhs();

    (* mhs)(spectra, D, & f);

    switch (options.candidate_printer) {
    case 0: // NORMAL:
        D.print(options.output()) << "\n";
        break;

    case 1: // PRETTY:
        D.pretty_print(options.output()) << "\n";
        break;

    case 2: // LATEX:
        D.latex_print(options.output()) << "\n";
        break;
    }

    return true;
}

int main (int argc, char ** argv) {
    t_diag_options options(argv[0]);


    if (!options.configure(argc, argv))
        return 1;

    options.debug() << options << std::endl;

    while (options.input().good()) {
        t_count_spectra spectra;
        spectra.read(options.input());

        if (options.input().eof())
            break;

        if (options.input().fail() || options.input().bad()) {
            std::cerr << "Problem reading spectra" << std::endl;
            return 1;
        }

        if (!do_stuff(spectra, options))
            return 1;
    }

    return 0;
}