#ifndef __INSTR_TRANSACTION_H__
#define __INSTR_TRANSACTION_H__

#include "types.h"


#ifdef NINSTR

#define _instr_transaction_gate_register_metadata(id, key, val)

#define _instr_transaction_start(tg_id, explicit_end)
#define _instr_transaction_end(tg_id)

#else

#ifdef __cplusplus
extern "C" {
#endif

  void _instr_transaction_gate_register_metadata(t_probe_id id,
                                                 const char * key, 
                                                 const char * val);

  void _instr_transaction_start(t_transaction_gate_id tg_id,
                                char explicit_end);

  void _instr_transaction_end(t_transaction_gate_id tg_id); 

#ifdef __cplusplus
}
#endif
#endif
#endif
