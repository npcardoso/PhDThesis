#ifndef __TYPES_H_64acb0307b5bdd05ed4e770872708d1501275a32__
#define __TYPES_H_64acb0307b5bdd05ed4e770872708d1501275a32__

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