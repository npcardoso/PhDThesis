#include "serialization/unserializers/json.h"
#include "utils/base64.h"
#include <boost/foreach.hpp>



  
using boost::property_tree::ptree;

t_json_observation_unserializer::t_json_observation_unserializer(t_observation_sink::t_ptr sink):sink(sink) {
}
  
void t_json_observation_unserializer::operator()(const boost::property_tree::ptree & tree,
                                                 bool skip_errors){

    boost::optional<t_construct_id> c_id;
    BOOST_FOREACH(const ptree::value_type &v, tree){
      if (v.first.size()) {
        //TODO: Raise exception
      }
      if((c_id = v.second.get_optional<t_construct_id>("tid")))
        read_transaction(v.second);
      else if((c_id = v.second.get_optional<t_construct_id>("oid")))
        read_oracle(v.second);
      else if((c_id = v.second.get_optional<t_construct_id>("pid")))
        read_probe(v.second);
    }
}


void t_json_observation_unserializer::read_probe(const boost::property_tree::ptree & tree) {
  t_probe_observation::t_ptr probe(new t_probe_observation());
  probe->c_id = tree.get<t_construct_id>("pid");
  probe->time = tree.get<t_time_interval>("t", -1);

  boost::optional<const ptree &> state = tree.get_child_optional("s");

  if(state) {
    BOOST_FOREACH(const ptree::value_type &v, *state){
      std::string str = base64_decode(v.second.data());
      probe->read_variable(str.c_str(), str.size());
    }
  }
  (*sink) << probe;
}

void t_json_observation_unserializer::read_oracle(const boost::property_tree::ptree & tree) {
  t_oracle_observation::t_ptr oracle(new t_oracle_observation());
  oracle->c_id = tree.get<t_construct_id>("oid");
  oracle->time = tree.get<t_time_interval>("t", -1);
  oracle->health = tree.get<float>("h");
  oracle->confidence = tree.get<float>("c", 1);
  (*sink) << oracle;
}

void t_json_observation_unserializer::read_transaction(const boost::property_tree::ptree & tree) {
}


