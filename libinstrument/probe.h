#ifndef __INSTR_PROBE_H__
#define __INSTR_PROBE_H__

#include "types.h"


#ifdef NINSTR

#define _instr_probe_register_metadata(id, key, val)

#define _instr_probe_observation_register(p_id)
#define _instr_probe_read(ptr, width)
#define _instr_probe_observation_commit()

#else

#ifdef __cplusplus
extern "C" {
#endif

  void _instr_probe_register_metadata(probe_id_t id, 
                                      const char * key, 
                                      const char * val);
  
  void _instr_probe_observation_register(probe_id_t p_id);

  void _instr_probe_read(void * ptr,
                         size_t width);
  
  void _instr_probe_observation_commit();

#ifdef __cplusplus
  }
#endif
#endif
#endif
