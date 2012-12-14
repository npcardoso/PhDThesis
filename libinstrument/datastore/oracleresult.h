#pragma once

#include "../types.h"

#include <boost/shared_ptr.hpp>

struct OracleResult {
public:
  time_interval_t time;
  oracle_id_t o_id; 
  float health, confidence;

public:
  typedef boost::shared_ptr<OracleResult> ptr;
  typedef boost::shared_ptr<const OracleResult> const_ptr;
  
  inline OracleResult(time_interval_t time, oracle_id_t o_id, float health, float confidence) : time(time), o_id(o_id), health(health), confidence(confidence) {}
  
  inline bool operator > (const time_interval_t & time) const {
    return this->time > time;
  }
  
  inline bool operator < (const time_interval_t & time) const {
    return this->time < time;
  }
};
