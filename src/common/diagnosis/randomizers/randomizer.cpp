#include "randomizer.h"

namespace std {
ostream & operator << (ostream & out, const diagnosis::randomizers::t_system & system) {
    return system.write(out);
}
}