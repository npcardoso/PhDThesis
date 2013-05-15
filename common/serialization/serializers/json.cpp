#include "serialization/serializers/json.h"

#include "utils/base64.h"
#include "types.h"

#include <boost/foreach.hpp>
#include <string>

using namespace instrumentation;

std::ostream & t_json_map::put(std::ostream & out) {
  if(first)
    out << '{';
  else
    out << ',';
  first = false;
  return out;
}

std::ostream & t_json_map::close(std::ostream & out) {
  if(first)
    out << '{';
  out << '}';
  first = true;
  return out;
}

std::ostream & t_json_array::put(std::ostream & out) {
  if(first)
    out << '[';
  else
    out << ',';
  first = false;
  return out;
}

std::ostream & t_json_array::close(std::ostream & out) {
  if(first)
    out << '[';
  out << ']';
  first = true;
  return out;
}

std::ostream & t_json_observation_serializer::timestamp(std::ostream & out,
                                                        t_time_interval t) {
  std::streamsize precision = out.precision(1024);
  out << t;
  out.precision(precision);
  return out;
}

std::ostream & t_json_observation_serializer::string(std::ostream & out,
                                                     std::string s) {
  out << '"' << s << '"';
  return out;
}

std::ostream & t_json_observation_serializer::key(std::ostream & out,
                                                  std::string k) {
  string(out, k) << ':';
  return out;
}


std::ostream & t_json_observation_serializer::observation_single(std::ostream & out,
                                                                 const t_observation_single & obs,
                                                                 t_element_group_writer & grp,
                                                                 const t_observation_serializer_options & opt) {
  if(opt.cid) {
    grp.put(out);
    key(out, "cid") << obs.c_id;
  }
  if(opt.timestamp) {
    grp.put(out);
    key(out, "t");
    timestamp(out, obs.time);
  }
  return out;
}

std::ostream & t_json_observation_serializer::observation_window(std::ostream & out,
                                                                 const t_observation_window & obs,
                                                                 t_element_group_writer & grp,
                                                                 const t_observation_serializer_options & opt) {
  if(opt.cid) {
    grp.put(out);
    key(out, "cid") << '[' << obs.c_id_start << ',' << obs.c_id_end << "]";
  }
  if(opt.timestamp) {
    grp.put(out);
    key(out, "t") << '[';
    timestamp(out, obs.time_start) << ',';
    timestamp(out, obs.time_end) << ']';
  }
  return out;
}

t_json_observation_serializer::t_json_observation_serializer() {
  static t_element_group_writer::t_ptr def_group = t_element_group_writer::t_ptr(new t_json_array());
  group = def_group;
}

t_json_observation_serializer::t_json_observation_serializer(t_element_group_writer::t_ptr group): group(group){
}

bool t_json_observation_serializer::operator () (std::ostream & out,
                                                 const t_oracle_observation::t_ptr & obs) {
  t_json_map tmp;
  group->put(out);

  observation_single(out, *obs, tmp, oracle_options);
  tmp.put(out);
  key(out, "h") << obs->health;
  tmp.put(out); 
  key(out, "c") << obs->confidence;
  tmp.close(out);

  return true;
}

bool t_json_observation_serializer::operator () (std::ostream & out,
                                                 const t_probe_observation::t_ptr & obs) {
  t_json_map tmp;
  group->put(out);

  observation_single(out, *obs, tmp, probe_options);
  if(obs->state) {
    tmp.put(out); 
    key(out, "s");
    t_json_array tmp2;

    t_count start = 0;
    t_count len = obs->state->offset_end[0];
    tmp2.put(out);
    string(out, base64_encode(obs->state->data, len, false));
    for(t_id i = 1; i < obs->state->n_vars; i++) {
      tmp2.put(out);
      start = obs->state->offset_end[i - 1];
      len = obs->state->offset_end[i] - start;
      string(out, base64_encode(obs->state->data + start, len, false));
    }
    tmp2.close(out);
  }
  tmp.close(out);
  return true;
}

bool t_json_observation_serializer::operator () (std::ostream & out,
                                                 const t_transaction_observation::t_ptr & obs) {
  t_json_map tmp;
  group->put(out);

  observation_window(out, *obs, tmp, transaction_options);

  t_element_group_writer::t_ptr arr_group(new t_json_array());

  if(obs->oracles.size()) {
    tmp.put(out);
    key(out, "o");

    t_json_observation_serializer tmp(*this);
    tmp.group = arr_group;
    BOOST_FOREACH(t_oracle_observation::t_ptr o, obs->oracles)
      tmp(out, o);
    tmp.close(out);
  }
  if(obs->probes.size()) {
    tmp.put(out); 
    key(out, "p");

    t_json_observation_serializer tmp(*this);
    tmp.group = arr_group;
    BOOST_FOREACH(t_probe_observation::t_ptr p, obs->probes) 
      tmp(out, p);
    tmp.close(out);
  }
  if(obs->transactions.size()) {
    tmp.put(out);
    key(out, "tr");

    t_json_observation_serializer tmp(*this);
    tmp.group = arr_group;
    BOOST_FOREACH(t_transaction_observation::t_ptr tr, obs->transactions) 
      tmp(out, tr);
    tmp.close(out);
  }
  tmp.close(out);
  return true;
}

t_observation_serializer::t_ptr t_json_observation_serializer::array() {
  t_element_group_writer::t_ptr tmp_group(new t_json_array());
  t_ptr tmp = t_ptr(new t_json_observation_serializer(tmp_group));
  return tmp;
}

void t_json_observation_serializer::close(std::ostream & out) {
  if(!group->empty())
    group->close(out);
}
