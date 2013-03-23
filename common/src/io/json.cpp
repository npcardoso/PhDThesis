#include "io/json.h"
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


#include <string>

using boost::property_tree::ptree;

namespace io {
namespace json {
  void observation(ptree & pt,
                   const t_oracle_observation & obs,
                   std::string prefix){
    pt.put(prefix + "cid", obs.c_id);
    //if(pid)
    //  pt.put(prefix + "pid", pid);
    pt.put(prefix + "t", obs.time);
    pt.put(prefix + "h", obs.health);
    pt.put(prefix + "c", obs.confidence);
  }

  void observation(ptree & pt,
                   const t_probe_observation & obs,
                   std::string prefix){
    pt.put(prefix + "cid", obs.c_id);
    //if(pid)
    //  pt.put(prefix + "pid", pid);
  }

  void request_header(ptree & pt, 
                      t_id id, 
                      t_instrument_proto::t_action action,
                      std::string prefix){
    pt.put(prefix + "id", id);
    pt.put(prefix + "a", action);
  }
}
}

