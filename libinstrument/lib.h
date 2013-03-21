#ifndef __INSTR_LIB_H__
#define __INSTR_LIB_H__

#include "datastore/datastore.h"

time_interval_t getTimeInterval();

DataStore * getDataStore();
void releaseDataStore();
#endif
