#ifndef __COMMON_INSTRUMENTATION_ORACLE_H__
#define __COMMON_INSTRUMENTATION_ORACLE_H__

#include "types.h"

#include "instrumentation/construct.h"
#include "instrumentation/observation.h"

namespace instrumentation {
class t_oracle_construct : public t_construct {
public:
};

class t_oracle_observation : public t_observation_single {
public:
    float health, confidence;

public:
    t_oracle_observation ();
    t_oracle_observation (t_time_interval time,
                          t_construct_id c_id,
                          float health,
                          float confidence);
};
}

#endif