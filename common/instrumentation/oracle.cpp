#include "instrumentation/oracle.h"

namespace instrumentation {
t_oracle_observation::t_oracle_observation() : t_observation_single(), health(1), confidence(1) {
}
  
t_oracle_observation::t_oracle_observation(t_time_interval time,
                                           t_construct_id c_id,
                                           float health,
                                           float confidence) : t_observation_single(time, c_id), health(health), confidence(confidence) {
}
  
}
