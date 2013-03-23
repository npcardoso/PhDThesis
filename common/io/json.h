#pragma once

#include <ostream>
#include "types.h"
#include "datastore/datastore.h"
#include "datastore/oracle.h"
#include "datastore/transaction.h"
#include "datastore/probe.h"

#include <boost/property_tree/ptree.hpp>



class t_instrument_proto {
public:
  typedef char t_action;
};

using namespace std;
namespace io {
namespace json {
  void observation(boost::property_tree::ptree & pt,
                   const t_oracle_observation & obs,
                   std::string prefix = std::string());
  
  void observation(boost::property_tree::ptree & pt,
                   const t_probe_observation & obs,
                   std::string prefix = std::string());
  
  void observation(boost::property_tree::ptree & pt,
                   const t_transaction_observation & obs,
                   std::string prefix = std::string());

  void request_header(boost::property_tree::ptree & pt, 
                      t_id id, 
                      t_instrument_proto::t_action action,
                      std::string prefix = std::string());
}
}
