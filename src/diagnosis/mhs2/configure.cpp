#include "configure.h"

#include <thread>

using namespace diagnosis::algorithms;

t_diag_options::t_diag_options (std::string app_name) : t_options(app_name, true, true) {
    // spectra
    print_spectra = false;
    candidate_printer = NORMAL;
    ambiguity_groups = false;
    conflict_ambiguity = false;

    add(t_opt('p', "print-spectra", false, false, "Prints the spectra read from input"));
    add(t_opt('P', "candidate-printer", true, false, "Selects a candidate printer (normal, pretty, latex)"));
    add(t_opt('a', "ambiguity", false, false, "Turns on ambiguity group removal"));
    add(t_opt('c', "conflict", false, false, "Turns on conflict ambiguity removal"));

    // cutoff
    cutoff = t_ptr<t_cutoff> (new t_cutoff());

    add(t_opt('t', "time", true, false, "Sets the time-based cutoff value (seconds)"));
    add(t_opt('D', "candidates", true, false, "Sets the candidate collection size cutoff value"));
    add(t_opt('d', "cardinality", true, false, "Sets the candidate cardinality cutoff value"));
    add(t_opt('l', "lambda", true, false, "Sets the lambda cutoff value"));

    // similarity
    add(t_opt('s', "similarity", true, false, "Sets heuristic (ochiai, jaccard, tarantula, random)"));

    // parallelization
    threads = std::thread::hardware_concurrency();
    parallelization = t_ptr<t_parallelization_factory_random> (new t_parallelization_factory_random(2));

    add(t_opt('L', "fork-level", true, false, "Sets the forking level"));
    add(t_opt('T', "threads", true, false, "Sets the number of threads"));
}

bool t_diag_options::short_opt (int c, char * param) {
    switch (c) {
    // Spectra Stuff
    case 'p': // Print spectra
        print_spectra = true;
        break;

    case 'P':

        if (!strcmp("normal", optarg))
            candidate_printer = NORMAL;
        else if (!strcmp("pretty", optarg))
            candidate_printer = PRETTY;
        else if (!strcmp("latex", optarg))
            candidate_printer = LATEX;
        else {
            std::cerr << "Invalid candidate printer: " << optarg << std::endl;
            return false;
        }

        break;

    case 'a': // Ambiguity groups
        ambiguity_groups = true;
        break;

    case 'c': // Conflict redundancy removal
        conflict_ambiguity = true;
        break;


    case 't': // Time cutoff

        if (!verb_strtof(optarg, cutoff->max_time, true))
            return false;

        cutoff->max_time *= 1e6;
        break;

    case 'D': // Max_candidates cutoff
        return verb_strtoi(optarg, cutoff->max_candidates, true);

    case 'd': // Candidate size cutoff
        return verb_strtoi(optarg, cutoff->max_candidate_size, true);

    case 'l': // Lambda cutoff
        return verb_strtof(optarg, cutoff->lambda, true);

    // Similarity stuff
    case 's':

        if (!strcmp("ochiai", optarg))
            similarity = t_ptr<t_similarity> (new t_ochiai());
        else if (!strcmp("tarantula", optarg))
            similarity = t_ptr<t_similarity> (new t_tarantula());
        else if (!strcmp("jaccard", optarg))
            similarity = t_ptr<t_similarity> (new t_jaccard());
        else if (!strcmp("random", optarg))
            similarity = t_ptr<t_similarity> (new t_random());
        else {
            std::cerr << "Invalid similarity coeficient: " << optarg << std::endl;
            return false;
        }

        break;

    // Parallelization stuff
    case 'L': // Fork level
        t_count fork_level;

        if (!verb_strtoi(optarg, fork_level, true))
            return false;

        parallelization = t_ptr<t_parallelization_factory_random> (new t_parallelization_factory_random(fork_level));
        break;

    case 'T':
        return verb_strtoi(optarg, threads, true);


    default:
        return t_options::short_opt(c, param);
    }

    return true;
}

std::ostream & t_diag_options::print (std::ostream & out) const {
    t_options::print(out);


    out << ", print-spectra: " << print_spectra;
    out << ", ambiguity: " << ambiguity_groups;
    out << ", conflict: " << conflict_ambiguity;

    out << ", threads: " << threads;
    out << ", cutoff_time: " << cutoff->max_time;
    out << ", cutoff_max_candidates: " << cutoff->max_candidates;
    out << ", cutoff_max_candidate_size: " << cutoff->max_candidate_size;
    out << ", cutoff_lambda: " << cutoff->lambda;


    return out;
}

t_ptr<algorithms::t_mhs> t_diag_options::get_mhs () const {
    t_ptr<t_mhs> mhs;

    if (threads > 1)
        mhs = t_ptr<t_mhs> (new t_mhs_parallel(parallelization, threads));
    else
        mhs = t_ptr<t_mhs> (new t_mhs());

    mhs->set_cutoff(cutoff);
    mhs->set_similarity(similarity);
    return mhs;
}