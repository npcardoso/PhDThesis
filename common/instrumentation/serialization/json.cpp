#include "instrumentation/serialization/json.h"

#include "utils/base64.h"


#include <string>

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


std::ostream & observation (std::ostream & out,
                            const t_observation_single & obs) {
  key(out, "cid") << obs.c_id << ',';
  key(out, "t");
  timestamp(out, obs.time);
  return out;
}

std::ostream & observation (std::ostream & out,
                            const t_observation_window & obs) {
  key(out, "cid") << '[' << obs.c_id_start << ',' << obs.c_id_end << "],";
  key(out, "t") << '[';
  timestamp(out, obs.time_start) << ',';
  timestamp(out, obs.time_end) << ']';
  return out;
}
}




#include "utils/debug.h"

t_json_serializer::t_ptr t_json_serializer::instance() {
  static t_ptr self(new t_json_serializer());
  debug("mutex value: (%lld)", self.get());
  return self;
}


std::ostream & t_json_serializer::observation(std::ostream & out,
                                              const t_oracle_observation & obs) {
  json::observation(out, (t_observation_single &)obs) << ',';
  json::key(out, "h") << obs.health << ',';
  json::key(out, "c") << obs.confidence;
  //if(pid)
  //  pt.put(prefix + "pid", pid);
  return out;
}

std::ostream & t_json_serializer::observation(std::ostream & out,
                                              const t_probe_observation & obs){

  json::observation(out, (t_observation_single &)obs);
  if(obs.state) {
    out << ',';
    json::key(out, "s");
    json::string(out, base64_encode(obs.state->data, obs.state->data_size()));
    out << ',';
    json::key(out, "v");
    out << '[' << obs.state->offset_end[0];
    for(t_id i = 1; i < obs.state->n_vars - 1; i++)
      out << ',' << obs.state->offset_end[i];
    out << ']';
  }
  //if(pid)
  //  pt.put(prefix + "pid", pid);
  return out;
}

std::ostream & t_json_serializer::observation(std::ostream & out,
                                              const t_transaction_observation & obs) {
  json::observation(out, (t_observation_window &)obs);
  if(obs.oracles.size()) {
    out << ',';
    json::key(out, "o");
    out << '[';
    t_serializer::array<t_oracle_observation::t_ptr>(out, obs.oracles, instance());
    out << ']';
  }
  if(obs.probes.size()) {
    out << ',';
    json::key(out, "p");
    out << '[';
    t_serializer::array<t_probe_observation::t_ptr>(out, obs.probes, instance());
    out << ']';
  }
  if(obs.transactions.size()) {
    out << ',';
    json::key(out, "tr");
    out << '[';
    t_serializer::array<t_transaction_observation::t_ptr>(out, obs.transactions, instance());
    out << ']';
  }
  return out;
}

std::ostream & t_json_serializer::observation_header(std::ostream & out) {
  out << '{';
  return out;
}

std::ostream & t_json_serializer::observation_separator(std::ostream & out) {
  out << ',';
  return out;

}

std::ostream & t_json_serializer::observation_footer(std::ostream & out) {
  out << '}';
  return out;

}

std::ostream & t_json_serializer::observation_request_header(std::ostream & out) {
  out << '[';
  return out;
}

std::ostream & t_json_serializer::observation_request_footer(std::ostream & out) {
  out << ']';
  return out;
}
