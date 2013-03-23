#ifndef __INSTR_ORACLE_H__
#define __INSTR_ORACLE_H__

#include "types.h"


#ifdef NINSTR

#define _instr_oracle_observation(c_id, health, confidence)

#else

#ifdef __cplusplus
extern "C" {
#endif

  void _instr_oracle_observation(t_construct_id c_id,
                                 t_health health,
                                 t_confidence confidence);

#ifdef __cplusplus
}
#endif
#endif
#endif

