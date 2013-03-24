#include "utils/debug.h"

#include "datastore/oracle.h"
#include "datastore/transaction.h"
#include "datastore/probe.h"
  
t_observation_sink_debug::t_observation_sink_debug(std::ostream & out):out(out) {
}

void t_observation_sink_debug::operator()(const t_observation::t_ptr & obs) {
  io_mutex.lock();
  if(t_probe_observation * ptr = dynamic_cast<t_probe_observation*>(obs.get())) {
    out << "New Probe Observation" << std::endl; 
  }
  else if(t_oracle_observation * ptr = dynamic_cast<t_oracle_observation*>(obs.get())) {
    out << "New Oracle Observation" << std::endl; 
  }
  else if(t_transaction_observation * ptr = dynamic_cast<t_transaction_observation*>(obs.get())) {
    out << "New Transaction Observation" << std::endl; 
  }
  io_mutex.unlock();
}
