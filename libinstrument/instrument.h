#ifndef __INSTRUMENT_H__
#define __INSTRUMENT_H__

#include "oracle.h"
#include "probe.h"
#include "transaction.h"

#include "types.h"

#include "utils/preprocessor.h"
#include <assert.h>

  /* Common */
#define _instr_str_aux(x) #x
#define _instr_str(x) _instr_str_aux(x)

#define _instr_expand_aux(item) item
#define _instr_expand(item) _instr_expand_aux item

#define _instr_metadata_location(register_fun) register_fun ("Location",( __FILE__  ":"  _instr_str(__LINE__)))

#define instr_void (;) 

  /* Oracle */

#ifdef NINSTR

#define instr_ometadata(key, value)

#define instr_oracle(condition, confidence, items...)

#define instr_assert_oracle(condition, confidence, items...) assert((condition))

#else

#define instr_ometadata(key, value) (_instr_oracle_register_metadata(0, (key), (value));)

#define instr_oracle(condition, confidence, items...) { \
  _instr_oracle_health(0, (condition), (confidence)); \
  FOREACH(_instr_expand, (_instr_metadata_location(instr_ometadata), items))\
}

#define instr_assert_oracle(condition, confidence, items...) {\
  instr_oracle((condition), (confidence), items)\
  assert((condition));\
}

#endif

  /* Probe */

#ifdef NINSTR

#define instr_pmetadata(key, value)
#define instr_pvar(item) 

#define instr_probe(items...)

#else

#define instr_pmetadata(key, value) (_instr_probe_register_metadata(0, (key), (value));)
#define instr_pvar(item) (_instr_probe_read((void *)&(item), sizeof(item));)

#define instr_probe(items...) { \
  _instr_probe_observation_register(0);\
  FOREACH(_instr_expand, (_instr_metadata_location(instr_pmetadata), items))\
  _instr_probe_observation_commit();\
}

#endif
  
  /* Transaction */

#ifdef NINSTR

#define instr_tmetadata(key, value)

#define transaction_start()
#define transaction_end()

#else

#define instr_tmetadata(key, value) (_instr_transaction_gate_register_metadata(0, (key), (value));)

#define instr_transaction_start(items...) { \
  _instr_transaction_start(0, 0);\
  FOREACH(_instr_expand, (_instr_metadata_location(instr_tmetadata), items))\
}

#define instr_transaction_end(items...) { \
  _instr_transaction_end(0);\
  FOREACH(_instr_expand, (_instr_metadata_location(instr_tmetadata),items))\
}

#endif
#endif
