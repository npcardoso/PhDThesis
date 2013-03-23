#include "main.h"

#include "io/graphviz.h"
#include "utils/debug.h"

#include <algorithm>
#include <iostream>
#include <sstream>
using namespace std;

t_thread_tracker * tracker;

void init() __attribute__((constructor));
void init() {
  debug("Instrumentation Init");
  
  tracker = new t_thread_tracker();
  tracker->start();
  debug("Instrumentation Init end");
}

void finish() __attribute__((destructor));
void finish() {
//  Graphviz(cout, *getDataStore());
  delete tracker;
  debug("Instrumentation Finalize");
}
