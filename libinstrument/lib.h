#ifndef __INSTR_LIB_H__
#define __INSTR_LIB_H__

#include "datastore/datastore.h"

t_time_interval getTimeInterval();

t_transaction_factory * getDataStore();
void releaseDataStore();
#endif
