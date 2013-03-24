#include "utils/debug.h"

#include "datastore/oracle.h"
#include "datastore/transaction.h"
#include "datastore/probe.h"

t_observation_sink_debug::t_observation_sink_debug(std::ostream & out):out(out) {
}

bool t_observation_sink_debug::operator()(const t_transaction_observation::t_ptr & obs) {
  io_mutex.lock();
  out << "New Probe Observation" << std::endl; 
  io_mutex.unlock();
  return true;

}

bool t_observation_sink_debug::operator()(const t_oracle_observation::t_ptr & obs) {
  io_mutex.lock();
  out << "New Oracle Observation" << std::endl; 
  io_mutex.unlock();
  return true;

}

bool t_observation_sink_debug::operator()(const t_probe_observation::t_ptr & obs) {
  io_mutex.lock();
  out << "New Transaction Observation" << std::endl; 
  io_mutex.unlock();
  return true;
}

bool t_construct_sink_debug::operator()(const t_transaction_construct::t_ptr & obs) {
  io_mutex.lock();
  out << "New Probe construct" << std::endl; 
  io_mutex.unlock();
  return true;
}

bool t_construct_sink_debug::operator()(const t_oracle_construct::t_ptr & obs) {
  io_mutex.lock();
  out << "New Oracle construct" << std::endl; 
  io_mutex.unlock();
  return true;
}

bool t_construct_sink_debug::operator()(const t_probe_construct::t_ptr & obs) {
  io_mutex.lock();
  out << "New Transaction construct" << std::endl; 
  io_mutex.unlock();
  return true;
}
