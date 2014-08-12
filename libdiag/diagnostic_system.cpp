#include "diagnostic_system.h"

namespace std {

std::ostream& operator<<(std::ostream & s,
                         const diagnostic::t_candidate_generator& cg) {
    s << cg.to_string();
    return s;
}

std::ostream& operator<<(std::ostream & s,
                         const diagnostic::t_candidate_ranker & cr) {
    s << cr.to_string();
    return s;
}

}
