#include "datastore.h"
#include "utils/debug.h"

#include <cassert>

// DataStore Stuff //

DataStore::DataStore(time_interval_t time,
                     pthread_t pthread_id,
                     size_t max_storage_size) {
  this->max_storage_size = max_storage_size;
  this->current_storage_size = 0;
  
  i_time = time;
  
  thread_count = 1;
  thread_mappings[pthread_id] = 0;
}

void DataStore::registerMainThread(time_interval_t time, pthread_t pthread_id) {
  thread_id_t id = id = thread_count++;
  thread_mappings[pthread_id] = id;

  thread_info[id] = ThreadInfo::ptr(new ThreadInfo (time - i_time, id));
  debug("Thread %ld(%ld): Registering Main", id, pthread_id);
}

void DataStore::registerThread(time_interval_t time, pthread_t  pthread_id, pthread_t pthread_launcher_id){
  assert(pthread_launcher_id != pthread_id);
  assert(thread_mappings.count(pthread_launcher_id) == 1);

  //Add thread mapping
  thread_id_t id = id = thread_count++;
  thread_mappings[pthread_id] = id;

  thread_id_t launcher_id = getThreadMapping(pthread_launcher_id);

  thread_info[id] = ThreadInfo::ptr(new ThreadInfo (time - i_time, launcher_id));
  debug("Thread %ld(%ld): Launched by %ld(%ld)", id, pthread_id, launcher_id, pthread_launcher_id);
}

void DataStore::registerThreadEnd(time_interval_t time, pthread_t pthread_id){
  thread_id_t id = getThreadMapping(pthread_id);

  debug("Thread %ld: End", id);

  thread_info[id]->end = time - i_time;
}

//Transaction Related
transaction_gate_id_t DataStore::registerTransactionGate() {
  debug("Registering Transaction Gate with tg_id %ld", transaction_gate_metadata.size());
  transaction_gate_metadata.push_back(instr_artifact_t());
  return transaction_gate_metadata.size() - 1;
}

void DataStore::registerTransactionGateMetadata(transaction_gate_id_t tg_id, string key, string value) {
  debug("Registering %s of Transaction Gate %ld: '%s'", key.c_str(), tg_id, value.c_str());
  assert(tg_id < transaction_gate_metadata.size());
  
  transaction_gate_metadata[tg_id][key] = value;
}


void DataStore::registerTransaction(time_interval_t time,
                                    pthread_t pthread_id,
                                    transaction_gate_id_t tg_id,
                                    bool explicit_end) {

  thread_id_t id = getThreadMapping(pthread_id);


  size_t obj_size = sizeof(Transaction);
  if(canStore(obj_size)) {
    Transaction::ptr tr(new Transaction(time - i_time, tg_id, explicit_end));

    thread_info[id]->pushTransaction(tr);
    
    current_storage_size += obj_size;

    debug("Thread %ld, Transaction Gate %ld: Registered Transaction, Storage: %f%%",
          id,
          tg_id,
          100.0 * current_storage_size / max_storage_size);
  }
  else {
    thread_info[id]->pushTransaction();
    
    debug("Thread %ld, Transaction Gate %ld: Dropped Transaction, Storage: Full(%f%%)",
          id,
          tg_id,
          100.0 * current_storage_size / max_storage_size);
  }
}

void DataStore::registerTransactionEnd(time_interval_t time,
                                       pthread_t pthread_id,
                                       transaction_gate_id_t gate) {

  thread_id_t id = getThreadMapping(pthread_id);

  thread_info[id]->popTransaction(time - i_time, gate);

  debug("Thread %ld, Transaction %ld: Registering Transaction End",
        id, -1l);
}


//Oracle Related
oracle_id_t DataStore::registerOracle() {
  debug("Registering Oracle with o_id %ld", oracle_metadata.size());
  
  oracle_metadata.push_back(instr_artifact_t());
  
  return oracle_metadata.size() - 1;
}

void DataStore::registerOracleMetadata(oracle_id_t id, string key, string value) {
  debug("Registering %s of Oracle %ld: '%s'", key.c_str(), id, value.c_str());
  assert(id < oracle_metadata.size());
  
  oracle_metadata[id][key] = value;
}


void DataStore::registerHealth(time_interval_t time,
                               pthread_t pthread_id,
                               oracle_id_t o_id,
                               float health,
                               float confidence) {

  thread_id_t id = getThreadMapping(pthread_id);

  size_t obj_size = sizeof(Transaction);
  if(canStore(obj_size)) {
    OracleResult::ptr o_res(new OracleResult(time - i_time, o_id, health, confidence));
    
    if(thread_info[id]->addOracleResult(o_res)) {
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
probe_id_t DataStore::registerProbe() {
  debug("Registering Probe with p_id %ld", probe_metadata.size());
  probe_metadata.push_back(instr_artifact_t());
  return probe_metadata.size() - 1;
}

void DataStore::registerProbeMetadata(probe_id_t id, string key, string value) {
  debug("Registering %s of Probe %ld: '%s'", key.c_str(), id, value.c_str());
  assert(id < probe_metadata.size());
  
  probe_metadata[id][key] = value;
}

void DataStore::registerObservation(time_interval_t time, pthread_t pthread_id, probe_id_t p_id) {
  thread_id_t id = getThreadMapping(pthread_id);

  debug("Thread %ld, Probe %ld: Registering observation", id, p_id);

  assert(thread_info.count(id) == 1);
  assert(observation_buffer.count(id) == 0);

  observation_buffer[id] = Observation::ptr(new Observation(time - i_time, p_id));
}

void DataStore::commitObservation(pthread_t pthread_id) {
  thread_id_t id = getThreadMapping(pthread_id);

  assert(observation_buffer.count(id) == 1);

  Observation::ptr obs = observation_buffer[id];

  size_t obj_size = sizeof(Observation) + obs->size();
  if(canStore(obj_size)) {
    if(thread_info[id]->addObservation(obs)){
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
  observation_buffer.erase(id);
}

void DataStore::discardObservation(pthread_t pthread_id) {
  thread_id_t id = getThreadMapping(pthread_id);

  assert(observation_buffer.count(id) == 1);

  observation_buffer.erase(id);
}

void DataStore::readVariable(pthread_t pthread_id, const void * ptr, size_t size){
  thread_id_t id = getThreadMapping(pthread_id);
  observation_buffer_t::iterator it = observation_buffer.find(id);

  assert(it != observation_buffer.end());

  debug("Thread %ld, Probe %ld: Reading Variable with size %ld",
        id,
        it->second->p_id,
        size);


  it->second->readVariable(ptr, size);
}

