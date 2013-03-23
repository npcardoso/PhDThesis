#ifndef __INSTR_PROBE_H__
#define __INSTR_PROBE_H__

#include "types.h"


#ifdef NINSTR

#define _instr_probe_register_metadata(id, key, val)

#define _instr_probe_observation(p_id, ...)

#else

#ifdef __cplusplus
extern "C" {
#endif

  void _instr_probe_metadata(t_probe_id id, 
                             const char * key, 
                             const char * val);

  void _instr_probe_observation(t_probe_id p_id, ...);

#ifdef __cplusplus
}
#endif
#endif
#endif
