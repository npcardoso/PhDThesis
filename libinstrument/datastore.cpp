#include "datastore.h"
#include "utils/debug.h"

#include <cassert>

// DataStore Stuff //

DataStore::DataStore(time_interval_t time,
                     pthread_t pthread_id,
                     size_t max_storage_size):
  observation_storage(max_storage_size),
  transaction_storage(max_storage_size) {
  this->max_storage_size = max_storage_size;
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

void DataStore::registerTransaction(time_interval_t time,
                                    pthread_t pthread_id,
                                    transaction_gate_id_t tg_id,
                                    bool explicit_end) {

  thread_id_t id = getThreadMapping(pthread_id);


  if(transaction_storage.canStore(sizeof(Transaction))) {
    Transaction::ptr tr(new Transaction(time - i_time, tg_id, explicit_end));
    tr->alloc_id = transaction_storage.store(tr, sizeof(Transaction), id);
    updateStorageCapacity();
    thread_info[id]->pushTransaction(tr);
    debug("Thread %ld, Transaction Gate %ld: Registered Transaction, Storage: %f%%",
          id,
          tg_id,
          100.0 * getStorageOccupancy() / max_storage_size);
  }
  else {
    thread_info[id]->pushTransaction();
    debug("Thread %ld, Transaction Gate %ld: Dropped Transaction, Storage: Full(%f%%)",
          id,
          tg_id,
          100.0 * getStorageOccupancy() / max_storage_size);
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

void DataStore::registerHealth(time_interval_t time,
                               pthread_t pthread_id,
                               oracle_id_t o_id,
                               float health,
                               float confidence) {

  thread_id_t id = getThreadMapping(pthread_id);

  debug("Thread %ld, Oracle %ld: Registering Oracle Result (Health: %f, Confidence: %f)",
        id, o_id, health, confidence);
}

//Observation Related

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

  if(observation_storage.canStore(obs->size())) {
    obs->alloc_id = observation_storage.store(obs, obs->size(), id);
    updateStorageCapacity();
    thread_info[id]->addObservation(obs);
    debug("Thread %ld, Probe %ld: Commited Observation with size %ld, Storage: %f%%",
          id,
          obs->p_id,
          obs->size(),
          100.0 * getStorageOccupancy() / max_storage_size);
  }
  else {
    debug("Thread %ld, Probe %ld: Dropped Observation with size %ld, Storage: Full(%f%%)",
          id,
          obs->p_id,
          obs->size(),
          100.0 * getStorageOccupancy() / max_storage_size);
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

