#include "configure.h"

#include <thread>

t_diag_options::t_diag_options (std::string app_name) : t_options(app_name, true, true) {
    has_confidence = false;
    print_spectra = false;
    ambiguity_groups = false;

    add(t_opt('p', "print-spectra", false, false, "Prints the spectra read from input"));
    add(t_opt('C', "has-confidence", false, false, "Enables confidence in the spectra"));
    add(t_opt('a', "ambiguity", false, false, "Turn on ambiguity groups simplification"));


    do_generation = false;
    fork_level = 0;
    stride = 0;
    threads = std::thread::hardware_concurrency();

    add(t_opt('g', "generate", false, false, "Enables candidate generation"));
    add(t_opt('t', "time", true, false, "Sets maximum candidate generation time"));
    add(t_opt('D', "candidates", true, false, "Sets maximum number of candidates"));
    add(t_opt('d', "cardinality", true, false, "Sets maximum candidate cardinality"));
    add(t_opt('L', "fork-level", true, false, "Sets the forking level"));
    add(t_opt('S', "stride", true, false, "Sets the stride factor"));
    add(t_opt('T', "threads", true, false, "Sets the stride factor"));


    do_ranking = false;
    barinel.use_confidence = false;
    barinel.use_fuzzy_error = false;

    add(t_opt('r', "rank", false, false, "Enables ranking (implies -g)"));
    add(t_opt('f', "fuzzy", false, false, "Enables fuzzy errors"));
    add(t_opt('c', "confidence", false, false, "Enables assertion confidence"));
}

bool t_diag_options::short_opt (int c, char * param) {
    switch (c) {
    // Spectra Stuff
    case 'p':
        print_spectra = true;
        break;

    case 'C':
        has_confidence = true;
        break;

    case 'a':
        ambiguity_groups = true;
        break;


    // Candidate Generation Stuff
    case 'g':
        do_generation = true;
        break;

    case 't':

        if (verb_strtof(optarg, mhs.max_time, true))
            return true;

        mhs.max_time *= 1e6;
        return false;

    case 'D':
        return verb_strtoi(optarg, mhs.max_candidates, true);

    case 'd':
        return verb_strtoi(optarg, mhs.max_candidate_size, true);


    // Parallelization Stuff
    case 'L':
        return verb_strtoi(optarg, fork_level, true);

    case 'S':
        return verb_strtoi(optarg, stride, true);

    case 'T':
        return verb_strtoi(optarg, threads, true);

        break;


    case 'r':
        do_ranking = true;
        do_generation = true;
        break;

    case 'f':
        barinel.use_fuzzy_error = true;
        break;

    case 'c':
        barinel.use_confidence = true;
        break;

    default:
        return t_options::short_opt(c, param);
    }

    return false;
}

std::ostream & t_diag_options::print (std::ostream & out) const {
    t_options::print(out);


    out << ", print-spectra: " << print_spectra;
    out << ", has-confidence: " << has_confidence;
    out << ", ambiguity: " << ambiguity_groups;

    out << ", generate: " << do_generation;

    if (do_generation) {
        out << ", time: " << mhs.max_time;
        out << ", candidates: " << mhs.max_candidates;
        out << ", cardinality: " << mhs.max_candidate_size;
        out << ", fork-level: " << fork_level;
        out << ", stride: " << stride;
        out << ", threads: " << threads;
    }

    out << ", rank: " << do_ranking;

    if (do_ranking) {
        out << ", fuzzy: " << barinel.use_fuzzy_error;
        out << ", confidence: " << barinel.use_confidence;
    }

    return out;
}