#include "transaction.h"

#include "main.h"
#include "utils/debug.h"
#include "utils/time.h"
#include "io/json.h"

extern "C"{
t_construct_id _instr_transaction_register(){
  t_construct::t_ptr construct(new t_transaction_construct());
  t_construct_id id = 0;//tracker->get()->register_construct(construct);
  return id;
}
}

void _instr_transaction_start(t_construct_id c_id) {
  t_transaction_observation::t_ptr obs(new t_transaction_observation(time_interval(), c_id));
  tracker->get()->observation(obs);
}
/* TODO: Temporary*/
#include <boost/thread/mutex.hpp>
boost::mutex io_mutex;

void _instr_transaction_end(t_construct_id c_id) {
  pthread_t id = pthread_self();
  t_transaction_factory::t_ptr factory = tracker->get();
  t_transaction_observation::t_ptr obs = factory->transaction_end(time_interval(), c_id);
  if(!factory->num_active()){
    io_mutex.lock();
    std::cerr << "{";
    io::json::observation(std::cerr, *obs);
    std::cerr << "}" << std::endl;
    io_mutex.unlock();
  }
}
