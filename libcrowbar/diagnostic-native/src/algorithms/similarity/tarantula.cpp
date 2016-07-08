#include "tarantula.h"

namespace diagnostic {
namespace algorithms {
t_score t_tarantula::similarity_coefficient (const t_count n[2][2]) const {
    if (!n[1][1])
        return 0;

    t_score tmp1 = n[1][1] / (t_score) (n[1][1] + n[0][1]);
    t_score tmp2 = 0;

    if (n[1][0])
        tmp2 = n[1][0] / (t_score) (n[1][0] + n[0][0]);

    return tmp1 / (tmp1 + tmp2);
}

std::ostream & t_tarantula::write (std::ostream & out) const {
    return out << "t_tarantula";
}
}
}