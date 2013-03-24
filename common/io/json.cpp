#include "io/json.h"

#include "utils/base64.h"

#include <boost/foreach.hpp>



#include <string>

using boost::property_tree::ptree;

namespace io {
namespace json {
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
  
  template <class I, class C>
  std::ostream & array(std::ostream & out,
                       const C & container,
                       std::string open = std::string(),
                       std::string close = std::string()) {
    bool first = true;
    out << '[';
    BOOST_FOREACH(I it, container) {
      if(!first)
        out << ',';
      out << open;
      observation(out, *it) << close;
      first = false;
    }
    out << ']';
    return out;
  }
  
  std::ostream & observation (std::ostream & out,
                             const t_observation_single & obs) {
    key(out, "cid") << obs.c_id << ',';
    key(out, "t") << obs.time;
    return out;
  }
  
  std::ostream & observation (std::ostream & out,
                             const t_observation_window & obs) {
    key(out, "cid") << '[' << obs.c_id_start << ',' << obs.c_id_end << "],";
    key(out, "t") << '[' << obs.time_start << ',' << obs.time_end << ']';
    return out;
  }
  
  std::ostream & observation(std::ostream & out,
                             const t_oracle_observation & obs) {
    observation(out, (t_observation_single &)obs) << ',';
    key(out, "h") << obs.health << ',';
    key(out, "c") << obs.confidence;
    //if(pid)
    //  pt.put(prefix + "pid", pid);
    return out;
  }

  std::ostream & observation(std::ostream & out,
                             const t_probe_observation & obs){
  
    observation(out, (t_observation_single &)obs);
    if(obs.state) {
      out << ',';
      key(out, "s");
      string(out, base64_encode(obs.state->data, obs.state->data_size()));
    }
    //if(pid)
    //  pt.put(prefix + "pid", pid);
    return out;
  }
  
  std::ostream & observation(ostream & out,
                             const t_transaction_observation & obs) {
    observation(out, (t_observation_window &)obs);
    if(obs.oracles.size()) {
      out << ",";
      key(out, "o");
      array<t_oracle_observation::t_ptr>(out, obs.oracles, "{", "}");
    }
    if(obs.probes.size()) {
      out << ",";
      key(out, "p");
      array<t_probe_observation::t_ptr>(out, obs.probes, "{", "}");
    }
    if(obs.transactions.size()) {
      out << ",";
      key(out, "tr");
      array<t_transaction_observation::t_ptr>(out, obs.transactions, "{", "}");
    }
    return out;
  }

  std::ostream & request_header(std::ostream & out,
                      t_id id,
                      char action){
//    pt.put(prefix + "id", id);
 //   pt.put(prefix + "a", action);
    return out;
  }
}
}

