#ifndef __INSTR_ORACLE_H__
#define __INSTR_ORACLE_H__

#include "../types.h"
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NPROBE
#define _instr_oracle_health(o_id, health, confidence)

#define oracle(pass, confidence)

#define assert_oracle(pass, confidence) assert(pass)

#else

  /* Oracle API */
  void _instr_oracle_health(oracle_id_t o_id,
                            float health,
                            float confidence);



#define oracle(pass, confidence) _instr_oracle_health(0, pass, confidence)

#define transaction_assert_oracle(pass, confidence) {\
  oracle(pass, confidence);\
  assert(pass);\
}

#endif

#ifdef __cplusplus
  }
#endif
#endif

