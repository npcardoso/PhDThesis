#ifndef __COMMON_SERIALIZATION_UNSERIALIZERS_JSON_H__
#define __COMMON_SERIALIZATION_UNSERIALIZERS_JSON_H__

#include "serialization/unserializer.h"

#include <boost/property_tree/ptree.hpp>

class t_json_observation_unserializer: public t_observation_unserializer {
public:
  DEFINE_BOOST_SHARED_PTRS(t_json_observation_unserializer);
  
  t_json_observation_unserializer(const boost::property_tree::ptree & tree);

  virtual bool operator >> (t_transaction_observation::t_ptr & obs);
  virtual bool operator >> (t_oracle_observation::t_ptr & obs);
  virtual bool operator >> (t_probe_observation::t_ptr & obs);

  virtual bool ended() const;
  inline virtual bool error() const {
    return error_;
  }
  
//  virtual ~t_json_observation_unserializer();

private:
  bool next();
  bool next_probe(const boost::property_tree::ptree & tree,
                  t_probe_observation & probe);
  bool is_array;
  bool error_;
  const boost::property_tree::ptree & tree;
  boost::property_tree::ptree::const_iterator iter;
};

#endif
