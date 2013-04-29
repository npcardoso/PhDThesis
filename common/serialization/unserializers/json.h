#ifndef __COMMON_SERIALIZATION_UNSERIALIZERS_JSON_H__
#define __COMMON_SERIALIZATION_UNSERIALIZERS_JSON_H__

#include "serialization/unserializer.h"

class t_json_observation_unserializer: public t_observation_unserializer {
public:
  DEFINE_BOOST_SHARED_PTRS(t_json_observation_unserializer);
  
  t_json_observation_unserializer(const std::string & data);

  virtual bool operator >> (t_transaction_observation::t_ptr & obs);
  virtual bool operator >> (t_oracle_observation::t_ptr & obs);
  virtual bool operator >> (t_probe_observation::t_ptr & obs);
  
  virtual ~t_json_observation_unserializer();

private:
  std::string data;
  t_count pos;
};

#endif
