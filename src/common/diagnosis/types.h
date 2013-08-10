#ifndef __DIAGNOSIS_TYPES_H__
#define __DIAGNOSIS_TYPES_H__

#include "../types.h"

#include "diagnosis/structs/trie.h"
#include "utils/mpreal.h"

namespace diagnosis {
// Arbitrary Precision
typedef mpfr::mpreal t_probability_mp;
typedef t_probability_mp t_goodness_mp;

// Double Precision
typedef double t_probability;
typedef t_probability t_goodness;
}
#endif