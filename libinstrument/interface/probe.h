#ifndef __INSTR_PROBE_H__
#define __INSTR_PROBE_H__

#include "../types.h"
#include "../utils/preprocessor.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NPROBE

#define probe(...)

#define _instr_probe_register_metadata(id, key, val)

#define _instr_probe_observation_register(p_id)
#define _instr_probe_read(ptr, width)
#define _instr_probe_observation_commit()

#else

  /* Probes API */
  void _instr_probe_register_metadata(probe_id_t id, 
                                      const char * key, 
                                      const char * val);
  
  void _instr_probe_observation_register(probe_id_t p_id);

  void _instr_probe_read(void * ptr,
                         size_t width);
  
  void _instr_probe_observation_commit();


#define _probe_read_pass(item) _instr_probe_read((void *)&(item), sizeof(item));

#define _probe_metadata_first(a, b) a
#define _probe_metadata_second(a, b) b

#define _probe_metadata_pass(item) _instr_probe_register_metadata(0, (_probe_metadata_first item), (_probe_metadata_second item));


#define hit_probe(){ \
  _instr_probe_observation_register(0);\
  _instr_probe_observation_commit();\
}

#define hit_probe_metadata(metadata...){ \
  _instr_probe_observation_register(0);\
  FOREACH(_probe_metadata_pass, (metadata))\
  _instr_probe_observation_commit();\
}

#define probe(state) { \
  _instr_probe_observation_register(0);\
  FOREACH(_probe_read_pass, state)\
  _instr_probe_observation_commit();\
}

#define probe_metadata(state, metadata...) { \
  _instr_probe_observation_register(0);\
  FOREACH(_probe_metadata_pass, (metadata))\
  FOREACH(_probe_read_pass, state)\
  _instr_probe_observation_commit();\
}


#define metadata_item(type, args...) (type, args)
#define pstate(v...) (v)

#endif

#ifdef __cplusplus
  }
#endif
#endif
