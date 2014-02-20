#ifndef __COMMON_SERIALIZATION_UNSERIALIZERS_JSON_H__
#define __COMMON_SERIALIZATION_UNSERIALIZERS_JSON_H__

#include "instrumentation/sink.h"

#include <boost/property_tree/ptree.hpp>

namespace instrumentation {
class t_json_observation_unserializer {
public:
    t_json_observation_unserializer (t_ptr<t_observation_sink> sink);

    void operator () (const boost::property_tree::ptree & tree,
                      bool skip_errors=true);


private:

    void read_probe (const boost::property_tree::ptree & tree,
                     t_ptr<t_observation_sink> sink);
    void read_oracle (const boost::property_tree::ptree & tree,
                      t_ptr<t_observation_sink> sink);
    void read_transaction (const boost::property_tree::ptree & tree,
                           t_ptr<t_observation_sink> sink);

    t_ptr<t_observation_sink> sink;
};
}
#endif