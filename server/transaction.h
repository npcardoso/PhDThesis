#pragma once

#include "../types.h"

#include <boost/shared_ptr.hpp>

struct Transaction {

public:
  typedef boost::shared_ptr<Transaction> ptr;
  typedef boost::shared_ptr<const Transaction> const_ptr;

  bool explicit_end;
  time_interval_t start, end;
  transaction_gate_id_t gate_start, gate_end;
  alloc_id_t alloc_id;

  inline Transaction(time_interval_t start,
                     transaction_gate_id_t gate_start,
                     bool explicit_end) {
    this->start = start;
    this->gate_start = gate_start;
    this->explicit_end = explicit_end;
    alloc_id = 0;
  }

  inline bool ended() const {
    return start < end;
  }
};
