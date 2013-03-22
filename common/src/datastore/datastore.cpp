#include "datastore/datastore.h"
#include "utils/debug.h"

#include <cassert>

// t_datastore Stuff //

t_datastore::t_datastore(t_time_interval time,
                     pthread_t pthread_id,
                     size_t max_storage_size) {
  this->max_storage_size = max_storage_size;
  this->current_storage_size = 0;
  
  i_time = time;
  
  thread_count = 1;
  thread_mappings[pthread_id] = 0;
}

void t_datastore::register_main_thread(t_time_interval time, pthread_t pthread_id) {
  t_thread_id id = id = thread_count++;
  thread_mappings[pthread_id] = id;

  thread_info[id] = t_thread_info::t_ptr(new t_thread_info (time - i_time, id));
  debug("Thread %ld(%ld): Registering Main", id, pthread_id);
}

void t_datastore::register_thread(t_time_interval time, pthread_t  pthread_id, pthread_t pthread_launcher_id){
  assert(pthread_launcher_id != pthread_id);
  assert(thread_mappings.count(pthread_launcher_id) == 1);

  //Add thread mapping
  t_thread_id id = id = thread_count++;
  thread_mappings[pthread_id] = id;

  t_thread_id launcher_id = thread_mapping(pthread_launcher_id);

  thread_info[id] = t_thread_info::t_ptr(new t_thread_info (time - i_time, launcher_id));
  debug("Thread %ld(%ld): Launched by %ld(%ld)", id, pthread_id, launcher_id, pthread_launcher_id);
}

void t_datastore::register_thread_end(t_time_interval time, pthread_t pthread_id){
  t_thread_id id = thread_mapping(pthread_id);

  debug("Thread %ld: End", id);

  thread_info[id]->end = time - i_time;
}

//Transaction Related
t_transaction_gate_id t_datastore::register_transaction_construct() {
  debug("Registering Transaction Gate with tg_id %ld", transaction_metadata.size());
  transaction_metadata.push_back(t_instr_artifact());
  return transaction_metadata.size() - 1;
}

void t_datastore::register_transaction_metadata(t_transaction_gate_id tg_id, string key, string value) {
  debug("Registering %s of Transaction Gate %ld: '%s'", key.c_str(), tg_id, value.c_str());
  assert(tg_id < transaction_metadata.size());
  
  transaction_metadata[tg_id][key] = value;
}


void t_datastore::register_transaction(t_time_interval time,
                                    pthread_t pthread_id,
                                    t_transaction_gate_id tg_id,
                                    bool explicit_end) {

  t_thread_id id = thread_mapping(pthread_id);


  size_t obj_size = sizeof(t_transaction_observation);
  if(can_store(obj_size)) {
    t_transaction_observation::t_ptr tr(new t_transaction_observation(time - i_time, tg_id, explicit_end));

    thread_info[id]->push_transaction(tr);
    
    current_storage_size += obj_size;

    debug("Thread %ld, Transaction Gate %ld: Registered Transaction, Storage: %f%%",
          id,
          tg_id,
          100.0 * current_storage_size / max_storage_size);
  }
  else {
    thread_info[id]->push_transaction();
    
    debug("Thread %ld, Transaction Gate %ld: Dropped Transaction, Storage: Full(%f%%)",
          id,
          tg_id,
          100.0 * current_storage_size / max_storage_size);
  }
}

void t_datastore::register_transaction_end(t_time_interval time,
                                       pthread_t pthread_id,
                                       t_transaction_gate_id gate) {

  t_thread_id id = thread_mapping(pthread_id);

  thread_info[id]->pop_transaction(time - i_time, gate);

  debug("Thread %ld, Transaction %ld: Registering Transaction End",
        id, -1l);
}


//Oracle Related
t_oracle_id t_datastore::register_oracle_construct() {
  debug("Registering Oracle with o_id %ld", oracle_metadata.size());
  
  oracle_metadata.push_back(t_instr_artifact());
  
  return oracle_metadata.size() - 1;
}

void t_datastore::register_oracle_metadata(t_oracle_id id, string key, string value) {
  debug("Registering %s of Oracle %ld: '%s'", key.c_str(), id, value.c_str());
  assert(id < oracle_metadata.size());
  
  oracle_metadata[id][key] = value;
}


void t_datastore::register_health(t_time_interval time,
                               pthread_t pthread_id,
                               t_oracle_id o_id,
                               float health,
                               float confidence) {

  t_thread_id id = thread_mapping(pthread_id);

  size_t obj_size = sizeof(t_oracle_observation);
  if(can_store(obj_size)) {
    t_oracle_observation::t_ptr obs(new t_oracle_observation(time - i_time, o_id, health, confidence));
    
    if(thread_info[id]->observation(obs)) {
      current_storage_size += obj_size;

      debug("Thread %ld, Oracle %ld: Registered Oracle Result (Health: %f, Confidence: %f), Storage: %f%%",
            id, o_id, health, confidence,
            100.0 * current_storage_size / max_storage_size);
    }
    else
      debug("Thread %ld, Oracle %ld: Ignored Oracle Result (Health: %f, Confidence: %f)",
            id, o_id, health, confidence);
  }
  else {
    debug("Thread %ld, Oracle %ld: Dropped Oracle Result (Health: %f, Confidence: %f), Storage: Full(%f%%)",
          id, o_id, health, confidence,
          100.0 * current_storage_size / max_storage_size);
  }


}

//Observation Related
t_probe_id t_datastore::register_probe_construct() {
  debug("Registering Probe with p_id %ld", probe_metadata.size());
  probe_metadata.push_back(t_instr_artifact());
  return probe_metadata.size() - 1;
}

void t_datastore::register_probe_metadata(t_probe_id id, string key, string value) {
  debug("Registering %s of Probe %ld: '%s'", key.c_str(), id, value.c_str());
  assert(id < probe_metadata.size());
  
  probe_metadata[id][key] = value;
}

void t_datastore::start_probe(t_time_interval time, pthread_t pthread_id, t_probe_id p_id) {
  t_thread_id id = thread_mapping(pthread_id);

  debug("Thread %ld, Probe %ld: Registering observation", id, p_id);

  assert(thread_info.count(id) == 1);
  assert(probe_buffer.count(id) == 0);

  probe_buffer[id] = t_probe_observation::t_ptr(new t_probe_observation(time - i_time, p_id));
}

void t_datastore::commit_observation(pthread_t pthread_id) {
  t_thread_id id = thread_mapping(pthread_id);

  assert(probe_buffer.count(id) == 1);

  t_probe_observation::t_ptr obs = probe_buffer[id];

  size_t obj_size = sizeof(t_probe_observation) + obs->size();
  if(can_store(obj_size)) {
    if(thread_info[id]->observation(obs)){
      current_storage_size += obj_size;

      debug("Thread %ld, Probe %ld: Commited Observation with size %ld, Storage: %f%%",
            id, obs->p_id, obs->size(),
            100.0 * current_storage_size / max_storage_size);
    }
    else
      debug("Thread %ld, Probe %ld: Ignored Observation with size %ld",
            id, obs->p_id, obs->size());
  }
  else {
    debug("Thread %ld, Probe %ld: Dropped Observation with size %ld, Storage: Full(%f%%)",
          id, obs->p_id, obs->size(),
          100.0 * current_storage_size / max_storage_size);
  }
  probe_buffer.erase(id);
}

void t_datastore::discard_observation(pthread_t pthread_id) {
  t_thread_id id = thread_mapping(pthread_id);

  assert(probe_buffer.count(id) == 1);

  probe_buffer.erase(id);
}

void t_datastore::read_variable(pthread_t pthread_id, const void * ptr, size_t size){
  t_thread_id id = thread_mapping(pthread_id);
  t_probe_buffer::iterator it = probe_buffer.find(id);

  assert(it != probe_buffer.end());

  debug("Thread %ld, Probe %ld: Reading Variable with size %ld",
        id,
        it->second->p_id,
        size);


  it->second->read_variable(ptr, size);
}

