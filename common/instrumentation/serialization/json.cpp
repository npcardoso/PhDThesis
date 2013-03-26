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

namespace json {
std::ostream & timestamp(std::ostream & out,
                         t_time_interval t) {
  std::streamsize precision = out.precision(1024);
  out << t;
  out.precision(precision);
  return out;
}

std::ostream & string(std::ostream & out,
                      std::string s) {
  out << '"' << s << '"';
  return out;
}

std::ostream & key(std::ostream & out,
                   std::string k) {
  string(out, k) << ':';
  return out;
}


std::ostream & observation_single (std::ostream & out,
                                   const t_observation_single & obs) {
  key(out, "cid") << obs.c_id << ',';
  key(out, "t");
  timestamp(out, obs.time);
  return out;
}

std::ostream & observation_window (std::ostream & out,
                                   const t_observation_window & obs) {
  key(out, "cid") << '[' << obs.c_id_start << ',' << obs.c_id_end << "],";
  key(out, "t") << '[';
  timestamp(out, obs.time_start) << ',';
  timestamp(out, obs.time_end) << ']';
  return out;
}
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

  json::observation_single(tmp.put(out), *obs);
  json::key(tmp.put(out), "h") << obs->health;
  json::key(tmp.put(out), "c") << obs->confidence;
  tmp.close(out);
  return true;
}

bool t_json_observation_serializer::operator << (const t_probe_observation::t_ptr & obs) {
  t_json_map tmp;
  group->put(out);
  
  json::observation_single(tmp.put(out), *obs);
  if(obs->state) {
    json::key(tmp.put(out), "s");
    json::string(out, base64_encode(obs->state->data, obs->state->data_size()));
    json::key(tmp.put(out), "v");

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

  json::observation_window(tmp.put(out), *obs);
  
  t_element_group::t_ptr arr_group(new t_json_array());

  if(obs->oracles.size()) {
    json::key(tmp.put(out), "o");
    
    t_json_observation_serializer tmp(out, arr_group);
    BOOST_FOREACH(t_oracle_observation::t_ptr o, obs->oracles)
      tmp << o;
    tmp.close();
  }
  if(obs->probes.size()) {
    json::key(tmp.put(out), "p");
    
    t_json_observation_serializer tmp(out, arr_group);
    BOOST_FOREACH(t_probe_observation::t_ptr p, obs->probes) 
      tmp << p;
    tmp.close();
  }
  if(obs->transactions.size()) {
    json::key(tmp.put(out), "tr");

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
