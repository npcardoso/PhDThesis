#ifndef __INSTR_DATASTORE_H__
#define __INSTR_DATASTORE_H__

#include "datastore/probe.h"
#include "datastore/transaction.h"
#include "datastore/oracle.h"

#include "datastore/threadinfo.h"

#include "types.h"

#include <boost/shared_ptr.hpp>
#include <map>
#include <string>


using namespace std;

class t_datastore {
public:
  /* Initial Time */
  
  t_time_interval i_time;

  /* Threads */
  
  typedef map<pthread_t, t_thread_id> t_thread_mappings;
  typedef map<t_thread_id, t_thread_info::t_ptr> t_thread_info_map;
  
  t_thread_id thread_count;

  t_thread_mappings thread_mappings;
  t_thread_info_map thread_info;

  /* Storage */
  
  size_t max_storage_size, current_storage_size;

  /* Observations */
  
  typedef map<t_thread_id, t_probe_observation::t_ptr> t_probe_buffer;
  
  t_probe_buffer probe_buffer;
  
  /* Instrumentation Artifacts Metadata */
  /* TODO: Refactor: move this logic elsewhere */
  typedef map<string, string> t_instr_artifact;
  typedef vector<t_instr_artifact> t_artifact_metadata_storage;
  
  t_artifact_metadata_storage transaction_metadata;
  t_artifact_metadata_storage probe_metadata;
  t_artifact_metadata_storage oracle_metadata;


  t_datastore(t_time_interval time,
              pthread_t pthread_id,
              size_t max_storage_size);

  //Thread Related
  void register_main_thread(t_time_interval time,
                            pthread_t thread_id);

  void register_thread(t_time_interval time,
                       pthread_t thread_id,
                       pthread_t launcher_id);

  void register_thread_end(t_time_interval time,
                           pthread_t thread_id);

  //Transaction Related
  t_transaction_gate_id register_transaction_construct();
  
  void register_transaction_metadata(t_transaction_gate_id tg_id, 
                                          string key, 
                                          string value);
  
  void register_transaction(t_time_interval time,
                           pthread_t pthread_id,
                           t_transaction_gate_id tg_id,
                           bool explicit_end);

  void register_transaction_end(t_time_interval time,
                              pthread_t pthread_id,
                              t_transaction_gate_id tg_id);

  //Oracle Related
  t_oracle_id register_oracle_construct();
  
  void register_oracle_metadata(t_oracle_id o_id, 
                                string key, 
                                string value);
  
  void register_health(t_time_interval time,
                       pthread_t pthread_id,
                       t_oracle_id o_id,
                       float health,
                       float confidence);


  //Observation Related
  t_probe_id register_probe_construct();
  
  void register_probe_metadata(t_probe_id p_id, 
                               string key, 
                               string value);

  void start_probe(t_time_interval time,
                   pthread_t t_id,
                   t_probe_id p_id);

  void commit_observation(pthread_t t_id);

  void discard_observation(pthread_t t_id);

  void read_variable(pthread_t t_id,
                     const void * ptr,
                     size_t size);


private:
  inline t_thread_id thread_mapping(pthread_t pthread_id) const {
    //Maybe we shouldn't be that strict. Ignore unkown threads with a debug output.
    assert(thread_mappings.count(pthread_id) == 1);
    return thread_mappings.find(pthread_id)->second;
  }

  inline size_t can_store(size_t bytes) const {
    return max_storage_size - current_storage_size >= bytes;
  }
};


#endif
