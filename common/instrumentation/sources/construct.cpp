#include "instrumentation/sources/construct.h"

bool t_construct_source::operator >> (t_transaction_construct::t_ptr & obs) {
  return false;
}

bool t_construct_source::operator >> (t_oracle_construct::t_ptr & obs) {
  return false;
}

bool t_construct_source::operator >> (t_probe_construct::t_ptr & obs) {
  return false;
}


