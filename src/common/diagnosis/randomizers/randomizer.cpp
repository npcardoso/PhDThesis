#include "randomizer.h"

namespace std {
ostream & operator << (ostream & out, const diagnosis::randomizers::t_spectra_randomizer & randomizer) {
    return randomizer.write(out);
}
}