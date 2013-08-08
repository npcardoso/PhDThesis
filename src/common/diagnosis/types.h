#ifndef __DIAGNOSIS_TYPES_H__
#define __DIAGNOSIS_TYPES_H__

#include "../types.h"
#include "utils/mpreal.h"

namespace diagnosis {
typedef mpfr::mpreal t_probability_mp;
typedef t_probability_mp t_goodness_mp;
}
#endif