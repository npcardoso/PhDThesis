#include "random.h"

#include <random>

namespace diagnostic {
namespace algorithms {
t_score t_random::operator () (const t_spectrum & spectrum,
                               t_component_id comp,
                               const t_spectrum_filter * filter) const {
    std::mt19937 gen;
    std::uniform_real_distribution<t_error> rand(0, 1);


    return rand(gen);
}
}
}