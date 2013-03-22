#pragma once

#include "../types.h"

#include <boost/shared_ptr.hpp>

struct t_transaction_observation {

public:
  typedef boost::shared_ptr<t_transaction_observation> t_ptr;
  typedef boost::shared_ptr<const t_transaction_observation> t_const_ptr;

  bool explicit_end;
  t_time_interval start, end;
  t_transaction_gate_id gate_start, gate_end;
  t_alloc_id alloc_id;

  inline t_transaction_observation(t_time_interval start,
                     t_transaction_gate_id gate_start,
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
