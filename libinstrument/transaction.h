#ifndef __INSTR_TRANSACTION_H__
#define __INSTR_TRANSACTION_H__

#include "types.h"


#ifdef NINSTR

#define _instr_transaction_start(c_id)
#define _instr_transaction_end(c_id)

#else

#ifdef __cplusplus
extern "C" {
#endif

  void _instr_transaction_start(t_construct_id c_id);

  void _instr_transaction_end(t_construct_id c_id); 

#ifdef __cplusplus
}
#endif
#endif
#endif
