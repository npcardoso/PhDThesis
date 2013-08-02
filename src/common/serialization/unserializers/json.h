#ifndef __COMMON_SERIALIZATION_UNSERIALIZERS_JSON_H__
#define __COMMON_SERIALIZATION_UNSERIALIZERS_JSON_H__

#include "instrumentation/sink.h"

#include <boost/property_tree/ptree.hpp>


class t_json_observation_unserializer {
public:
    DEFINE_BOOST_SHARED_PTRS(t_json_observation_unserializer);

    t_json_observation_unserializer (instrumentation::t_observation_sink::t_ptr sink);

    void operator () (const boost::property_tree::ptree & tree,
                      bool skip_errors=true);


private:

    void read_probe (const boost::property_tree::ptree & tree,
                     instrumentation::t_observation_sink::t_ptr sink);
    void read_oracle (const boost::property_tree::ptree & tree,
                      instrumentation::t_observation_sink::t_ptr sink);
    void read_transaction (const boost::property_tree::ptree & tree,
                           instrumentation::t_observation_sink::t_ptr sink);

    instrumentation::t_observation_sink::t_ptr sink;
};

#endif