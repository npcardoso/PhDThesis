#ifndef __INSTRUMENT_H__
#define __INSTRUMENT_H__

#include "oracle.h"
#include "probe.h"
#include "transaction.h"

#include "utils/preprocessor.h"
#include <assert.h>
#ifdef __cplusplus
extern "C" {
#endif

  /* Common */
#define _instr_str_aux(x) #x
#define _instr_str(x) _instr_str_aux(x)

#define _instr_expand_aux(item) item
#define _instr_expand(item) _instr_expand_aux item

#define _instr_metadata_location(register_fun) register_fun ("Location",( __FILE__  ":"  _instr_str(__LINE__)))

#define instr_void 0 

#ifdef NINSTR

#define instr_metadata(key, value)

#else

void _instr_metadata (t_construct_id c_id,
                      const char * key,
                      const char * val);


#define instr_metadata(key, value) _instr_metadata (0, (key), (value))

#endif

  /* Oracle */

#ifdef NINSTR


#define instr_oracle(condition, confidence)

#define instr_assert_oracle(condition, confidence) assert((condition))

#else


#define instr_oracle(condition, confidence) _instr_oracle_observation(0, (condition), (confidence))

#define instr_assert_oracle(condition, confidence){\
  instr_oracle((condition), (confidence));\
  assert((condition))\
}

#endif

  /* Probe */

#ifdef NINSTR

#define instr_pvar(item) 

#define instr_probe(items...)

#else

#define instr_pvar(item) sizeof(item), ((void *)&(item))

#define instr_probe(items...) _instr_probe_observation(0, items, 0, 0)

#endif
  
  /* Transaction */

#ifdef NINSTR

#define transaction_start()

#define transaction_end()

#else

#define instr_transaction_start() _instr_transaction_start(0)

#define instr_transaction_end() _instr_transaction_end(0)

#endif
#ifdef __cplusplus
}
#endif
#endif
