#include "oracle.h"

#include "main.h"
#include "utils/debug.h"
#include "utils/time.h"

extern "C"{
t_construct_id _instr_oracle_register(){
  t_oracle_construct::t_ptr construct(new t_oracle_construct());
  register_construct(construct);
  return construct->c_id;
}
}

void _instr_oracle_observation(t_construct_id c_id,
                               t_health health,
                               t_confidence confidence) {
  t_oracle_observation::t_ptr obs(new t_oracle_observation(time_interval(), c_id, health, confidence));
  (*tracker->get()) << obs;
}
