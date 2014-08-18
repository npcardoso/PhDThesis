#include "replay.h"

#include "../../structs/count_spectra.h"

#include <fstream>

using namespace std;

namespace diagnostic {
namespace benchmark {
t_spectra * t_replay::operator () (std::mt19937 & gen,
                                   t_candidate & correct_candidate) {
    t_spectra * spectra = new t_count_spectra();


    while (begin() != end()) {
        ifstream spectra_in(begin()->first);
        ifstream correct_in(begin()->second);
        t_candidate correct;
        std::cerr << "Here!" << std::endl;
        *spectra = t_count_spectra();
        spectra_in >> *spectra;
        correct_in >> correct;

        std::cerr << *spectra << correct;
        std::cerr << !spectra_in.fail() << !correct_in.fail();

        erase(begin());

        if (!spectra_in.fail() && !correct_in.fail()) {
            correct_candidate = correct;
            return spectra;
        }
    }

    return NULL;
}
}
}
