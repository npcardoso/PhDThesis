#include "ochiai.h"

namespace diagnostic {
namespace algorithms {
t_score t_ochiai::similarity_coefficient (const t_count n[2][2]) const {
    if (!n[1][1])
        return 0;

    t_score tmp = ((n[1][1] + n[0][1]) * (n[1][1] + n[1][0]));
    return n[1][1] / sqrt(tmp);
}

std::ostream & t_ochiai::write (std::ostream & out) const {
    return out << "t_ochiai";
}
}
}