#include "serialization/unserializers/json.h"
#include "utils/base64.h"
#include <boost/foreach.hpp>



  
using boost::property_tree::ptree;
t_json_observation_unserializer::t_json_observation_unserializer(const boost::property_tree::ptree & tree) : tree(tree), iter(tree.begin()) {
  error_ = false;
  next();
}

bool t_json_observation_unserializer::next_probe(const boost::property_tree::ptree & tree,
                                                 t_probe_observation & probe) {
    
    t_construct_id cid = tree.get<t_construct_id>("cid");
    t_time_interval time = tree.get<t_time_interval>("t", -1);
    boost::optional<const ptree &> state = tree.get_child_optional("s");

    t_probe_observation tmp_probe(cid, time);
    
    if(state) {
      BOOST_FOREACH(const ptree::value_type &v, *state){
        std::string str = base64_decode(v.second.data());
        tmp_probe.read_variable(str.c_str(), str.size());
      }
    }
//    probe = tmp_probe;
    return true;
}

bool t_json_observation_unserializer::next() {
  t_probe_observation probe(0,0);
  next_probe(iter->second, probe);
  if (iter->first.size()) {
    error_ = true;
    return true;
  }

  return true;
}

bool t_json_observation_unserializer::ended() const {
  return false;
}

bool t_json_observation_unserializer::operator >> (t_transaction_observation::t_ptr & obs){
  return false;
}
bool t_json_observation_unserializer::operator >> (t_oracle_observation::t_ptr & obs) {
  return false;
}
bool t_json_observation_unserializer::operator >> (t_probe_observation::t_ptr & obs) {
  return false;
}
