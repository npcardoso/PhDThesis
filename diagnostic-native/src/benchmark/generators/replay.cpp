#include "replay.h"

#include "../../structs/count_spectrum.h"

#include <fstream>

using namespace std;

namespace diagnostic {
namespace benchmark {
t_spectrum * t_replay::operator () (std::mt19937 & gen,
                                   t_candidate & correct_candidate) {
    t_spectrum * spectrum = new t_count_spectrum();


    while (begin() != end()) {
        ifstream spectrum_in(begin()->first);
        ifstream correct_in(begin()->second);
        t_candidate correct;
        std::cerr << "Here!" << std::endl;
        *spectrum = t_count_spectrum();
        spectrum_in >> *spectrum;
        correct_in >> correct;

        std::cerr << *spectrum << correct;
        std::cerr << !spectrum_in.fail() << !correct_in.fail();

        erase(begin());

        if (!spectrum_in.fail() && !correct_in.fail()) {
            correct_candidate = correct;
            return spectrum;
        }
    }

    return NULL;
}
}
}
