#include "instrumentation/sinks/construct.h"

bool t_construct_sink::operator()(const t_transaction_construct::t_ptr & obs) {
  return false;
}

bool t_construct_sink::operator()(const t_oracle_construct::t_ptr & obs) {
  return false;
}

bool t_construct_sink::operator()(const t_probe_construct::t_ptr & obs) {
  return false;
}


