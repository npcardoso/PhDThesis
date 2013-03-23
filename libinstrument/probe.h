#ifndef __INSTR_PROBE_H__
#define __INSTR_PROBE_H__

#include "types.h"


#ifdef NINSTR

#define _instr_probe_observation(c_id, ...)

#define _instr_hit_probe_observation(c_id)

#else

#ifdef __cplusplus
extern "C" {
#endif

  void _instr_probe_observation(t_construct_id c_id, ...);

  void _instr_hit_probe_observation(t_construct_id c_id);

#ifdef __cplusplus
}
#endif
#endif
#endif
