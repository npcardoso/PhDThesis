#ifndef __INSTR_ORACLE_H__
#define __INSTR_ORACLE_H__

#include "types.h"


#ifdef NINSTR

#define _instr_oracle_register_metadata(id, key, val)

#define _instr_oracle_health(o_id, health, confidence)


#else

#ifdef __cplusplus
extern "C" {
#endif

  void _instr_oracle_register_metadata(t_oracle_id id,
                                       const char * key,
                                       const char * val);

  void _instr_oracle_health(t_oracle_id o_id,
                            float health,
                            float confidence);

#ifdef __cplusplus
}
#endif
#endif
#endif

