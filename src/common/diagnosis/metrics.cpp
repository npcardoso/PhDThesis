#include "metrics.h"
namespace diagnosis {
namespace metrics {
t_diagnosis_quality quality (t_diagnosis_cost cost, t_count total) {
    assert(total > 0);
    assert(total > cost);

    if (cost == 0)
        return 1;

    return 1 - (cost / (total - 1));
}
}
}