#ifndef __INSTR_TRANSACTION_H__
#define __INSTR_TRANSACTION_H__

#include "../types.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NPROBE
#define _instr_transaction_start(tg_id, explicit_end)

#define _instr_transaction_end(tg_id)

#define transaction_start()

#define transaction_end()

#else

  /* Transactions API */
  void _instr_transaction_start(transaction_gate_id_t tg_id,
                                char explicit_end);

  void _instr_transaction_end(transaction_gate_id_t tg_id);


#define transaction_start() _instr_transaction_start(0, 0)

#define transaction_end() _instr_transaction_end(0)

#endif

#ifdef __cplusplus
  }
#endif
#endif
