#include "configure.h"

#include "diagnosis/heuristics/similarity.h"
#include "diagnosis/heuristics/sort.h"

t_heuristic t_mhs_options::default_heuristic () {
    t_heuristic heuristic;


    heuristic.push(new heuristics::t_ochiai());
    heuristic.push(new heuristics::t_sort());
    return heuristic;
}

t_mhs_options::t_mhs_options (std::string app_name) : t_options(app_name, true, true), mhs(default_heuristic()) {
    has_confidence = false;
    print_spectra = false;
    ambiguity_groups = false;

    add(t_opt('p', "print-spectra", false, false, "Prints the spectra read from input"));
    add(t_opt('C', "has-confidence", false, false, "Enables confidence in the spectra"));
    add(t_opt('a', "ambiguity", false, false, "Turn on ambiguity groups simplification"));


    do_generation = false;
    mpi_level = 0;
    mpi_stride = 0;
    mpi_buffer = MPI_BUFFER;
    mpi_hierarchical = false;

    add(t_opt('g', "generate", false, false, "Enables candidate generation"));
    add(t_opt('t', "time", true, false, "Sets maximum candidate generation time"));
    add(t_opt('D', "candidates", true, false, "Sets maximum number of candidates"));
    add(t_opt('d', "cardinality", true, false, "Sets maximum candidate cardinality"));
    add(t_opt('B', "mpi-buffer", true, false, "Sets the buffer size for reduce task"));
    add(t_opt('L', "mpi-level", true, false, "Sets the forking level"));
    add(t_opt('S', "mpi-stride", true, false, "Sets the stride factor"));
    add(t_opt('H', "mpi-hierarchical", false, false, "Enables hierarchical reduce"));


    do_ranking = false;
    use_confidence = false;
    use_fuzzy_error = false;

    add(t_opt('r', "rank", false, false, "Enables ranking (implies -g)"));
    add(t_opt('f', "fuzzy", false, false, "Enables fuzzy errors"));
    add(t_opt('c', "confidence", false, false, "Enables assertion confidence"));
}

bool t_mhs_options::short_opt (int c, char * param) {
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


    // MPI Stuff
    case 'B':
        return verb_strtoi(optarg, mpi_buffer, true);

    case 'L':
        return verb_strtoi(optarg, mpi_level, true);

    case 'S':
        return verb_strtoi(optarg, mpi_stride, true);

    case 'H':
        mpi_hierarchical = true;
        break;


    case 'r':
        do_ranking = true;
        do_generation = true;
        break;

    case 'f':
        use_fuzzy_error = true;
        break;

    case 'c':
        use_confidence = true;
        break;

    default:
        return t_options::short_opt(c, param);
    }

    return false;
}

std::ostream & t_mhs_options::print (std::ostream & out) const {
    t_options::print(out);


    out << ", print-spectra: " << print_spectra;
    out << ", has-confidence: " << has_confidence;
    out << ", ambiguity: " << ambiguity_groups;

    out << ", generate: " << do_generation;

    if (do_generation) {
        out << ", time: " << mhs.max_time;
        out << ", candidates: " << mhs.max_candidates;
        out << ", cardinality: " << mhs.max_candidate_size;
        out << ", mpi-buffer: " << mpi_buffer;
        out << ", mpi-level: " << mpi_level;
        out << ", mpi-stride: " << mpi_stride;
        out << ", mpi-hierarchical: " << mpi_hierarchical;
    }

    out << ", rank: " << do_ranking;

    if (do_ranking) {
        out << ", fuzzy: " << use_fuzzy_error;
        out << ", confidence: " << use_confidence;
    }

    return out;
}