#include "instrumentation/serialization/json.h"

#include "utils/base64.h"

#include <boost/foreach.hpp>
#include <string>

std::ostream & t_json_map::put(std::ostream & out) {
  if(first)
    out << '{';
  else
    out << ',';
  first = false;
  return out;
}

void t_json_map::close(std::ostream & out) {
  if(first)
    out << '{';
  out << '}';
  first = true;
}

std::ostream & t_json_array::put(std::ostream & out) {
  if(first)
    out << '[';
  else
    out << ',';
  first = false;
  return out;
}

void t_json_array::close(std::ostream & out) {
  if(first)
    out << '[';
  out << ']';
  first = true;
}

std::ostream & t_json_observation_serializer::timestamp(t_time_interval t) {
  std::streamsize precision = out.precision(1024);
  out << t;
  out.precision(precision);
  return out;
}

std::ostream & t_json_observation_serializer::string(std::string s) {
  out << '"' << s << '"';
  return out;
}

std::ostream & t_json_observation_serializer::key(std::string k) {
  string(k) << ':';
  return out;
}


std::ostream & t_json_observation_serializer::observation_single(const t_observation_single & obs) {
  key("cid") << obs.c_id << ',';
  key("t");
  timestamp(obs.time);
  return out;
}

std::ostream & t_json_observation_serializer::observation_window(const t_observation_window & obs) {
  key("cid") << '[' << obs.c_id_start << ',' << obs.c_id_end << "],";
  key("t") << '[';
  timestamp(obs.time_start) << ',';
  timestamp(obs.time_end) << ']';
  return out;
}

t_json_observation_serializer::t_json_observation_serializer(std::ostream & out): out(out) {
  static t_element_group::t_ptr def_group = t_element_group::t_ptr(new t_element_group());
  group = def_group;
}

t_json_observation_serializer::t_json_observation_serializer(std::ostream & out, t_element_group::t_ptr group): out(out), group(group){
}

bool t_json_observation_serializer::operator << (const t_oracle_observation::t_ptr & obs) {
  t_json_map tmp;
  group->put(out);
  
  tmp.put(out);
  observation_single(*obs);
  tmp.put(out);
  key("h") << obs->health;
  tmp.put(out); 
  key("c") << obs->confidence;
  tmp.close(out);
  
  return true;
}

bool t_json_observation_serializer::operator << (const t_probe_observation::t_ptr & obs) {
  t_json_map tmp;
  group->put(out);

  tmp.put(out); 
  observation_single(*obs);
  if(obs->state) {
    tmp.put(out); 
    key("s");
    string(base64_encode(obs->state->data, obs->state->data_size()));
    tmp.put(out); 
    key("v");

    t_json_array tmp2;
    for(t_id i = 0; i < obs->state->n_vars - 1; i++)
      tmp2.put(out) << obs->state->offset_end[i];
    tmp2.close(out);
  }
  tmp.close(out);
  return true;
}

bool t_json_observation_serializer::operator << (const t_transaction_observation::t_ptr & obs) {
  t_json_map tmp;
  group->put(out);
  
  tmp.put(out); 
  observation_window(*obs);
  
  t_element_group::t_ptr arr_group(new t_json_array());

  if(obs->oracles.size()) {
    tmp.put(out);
    key("o");
    
    t_json_observation_serializer tmp(out, arr_group);
    BOOST_FOREACH(t_oracle_observation::t_ptr o, obs->oracles)
      tmp << o;
    tmp.close();
  }
  if(obs->probes.size()) {
    tmp.put(out); 
    key("p");
    
    t_json_observation_serializer tmp(out, arr_group);
    BOOST_FOREACH(t_probe_observation::t_ptr p, obs->probes) 
      tmp << p;
    tmp.close();
  }
  if(obs->transactions.size()) {
    tmp.put(out);
    key("tr");

    t_json_observation_serializer tmp(out, arr_group);
    BOOST_FOREACH(t_transaction_observation::t_ptr tr, obs->transactions) 
      tmp << tr;
    tmp.close();
  }
  tmp.close(out);
  return true;
}
  
t_observation_serializer::t_ptr t_json_observation_serializer::array() {
  t_element_group::t_ptr tmp_group(new t_json_array());
  t_ptr tmp = t_ptr(new t_json_observation_serializer(group->put(out), tmp_group));
  return tmp;
}

t_json_observation_serializer::~t_json_observation_serializer() {
  close();
}

void t_json_observation_serializer::close() {
  if(!group->empty())
    group->close(out);
}
