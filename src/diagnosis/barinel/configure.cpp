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
    mpi_level = 0;
    mpi_stride = 0;
    mpi_buffer = MPI_BUFFER;
    mpi_hierarchical = false;

    fuzzinel = false;

    has_confidence = false;
    print_spectra = false;
    ambiguity_groups = false;

    // add(t_opt('p', "partition", true, false, "Sets the partition to compute (format  <p>:<np>)"));
    add(t_opt('p', "print-spectra", false, false, "Prints the spectra read from input"));
    add(t_opt('b', "mpi-buffer", true, false, "Sets the buffer size for reduce task"));
    add(t_opt('l', "mpi-level", true, false, "Sets the forking level"));
    add(t_opt('s', "mpi-stride", true, false, "Sets the stride factor"));
    add(t_opt('H', "mpi-hierarchical", false, false, "Enables hierarchical reduce"));
    add(t_opt('t', "time", true, false, "Sets maximum computation time"));
    add(t_opt('d', "candidates", true, false, "Sets maximum number of candidates"));
    add(t_opt('c', "cardinality", true, false, "Sets maximum candidate cardinality"));
    add(t_opt('f', "fuzzinel", false, false, "Enables fuzzinel diagnosis"));
    add(t_opt('a', "ambiguity", false, false, "Turn on ambiguity groups simplification"));
    add(t_opt('C', "confidence", false, false, "Enables confidence in the spectra"));
    add(t_opt('B', "barinel", false, false, "Enables confidence in the spectra"));
}

bool t_mhs_options::short_opt (int c, char * param) {
    switch (c) {
    case 't':

        if (verb_strtof(optarg, mhs.max_time, true))
            return true;

        mhs.max_time *= 1e6;
        return false;


    case 'd':
        return verb_strtoi(optarg, mhs.max_candidates, true);

    case 'c':
        return verb_strtoi(optarg, mhs.max_candidate_size, true);

    case 'l':
        return verb_strtoi(optarg, mpi_level, true);

    case 's':
        return verb_strtoi(optarg, mpi_stride, true);

    case 'b':
        return verb_strtoi(optarg, mpi_buffer, true);

    case 'C':
        has_confidence = true;
        break;

    case 'p':
        print_spectra = true;
        break;

    case 'f':
        fuzzinel = true;
        use_confidence = true;
        fuzzy_error = true;
        break;

    case 'B':
        fuzzinel = true;
        use_confidence = false;
        fuzzy_error = false;
        break;

    case 'a':
        ambiguity_groups = true;
        break;

    case 'H':
        mpi_hierarchical = true;
        break;

    default:
        return t_options::short_opt(c, param);
    }

    return false;
}

std::ostream & t_mhs_options::print (std::ostream & out) const {
    t_options::print(out);


    out << ", MPI_level: " << mpi_level;
    out << ", MPI_stride: " << mpi_stride;
    out << ", MPI_buffer: " << mpi_buffer;
    out << ", MPI_hierarchical: " << mpi_hierarchical;
    out << ", Cardinality: " << mhs.max_candidate_size;
    out << ", Candidates: " << mhs.max_candidates;
    out << ", Time: " << mhs.max_time;
    out << ", Fuzzinel: " << fuzzinel;
    out << ", Ambiguity_Groups: " << ambiguity_groups;
    out << ", Has_Confidence: " << has_confidence;
    out << ", Print_Spectra: " << print_spectra;
    return out;
}