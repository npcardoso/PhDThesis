#ifndef __CONVERTERS_OBSERVATION_GRAPHVIZ_H__
#define __CONVERTERS_OBSERVATION_GRAPHVIZ_H__

#include "instrumentation/transaction.h"

namespace converters {
class t_observations_to_graphviz {
public:
    void operator () (const instrumentation::t_transaction_observation & tr,
                      std::ostream & out);

    void start (std::ostream & out);
    void end (std::ostream & out);

private:
    void operator () (const instrumentation::t_probe_observation & p,
                      std::ostream & out);
    void operator () (const instrumentation::t_oracle_observation & o,
                      std::ostream & out);
    void rgba (float r, float g, float b, float a,
               std::ostream & out);
    t_id oracle_id;
    t_id probe_id;
    t_id transaction_id;
};
}
#endif