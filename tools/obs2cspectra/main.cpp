#include "utils/debug.h"
#include <iostream>
#include <string>
#include "instrumentation/sinks/collector.h"
#include "converters/observation_spectra.h"
#include "serialization/unserializers/json.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#define BUF_SIZE 102400
using namespace instrumentation;
using namespace instrumentation::sinks;

int main() {
  try {
    char tmp[BUF_SIZE];
    std::cin.getline (tmp, BUF_SIZE);


    boost::property_tree::ptree tree;
    std::stringstream stream(tmp);
    try{
      read_json(stream, tree);
    }
    catch (boost::property_tree::json_parser::json_parser_error & e) {
      std::cout << stream.str();
      std::cerr << e.what() << std::endl;
    }
    
    t_observation_collector_sink::t_ptr collector(new t_observation_collector_sink());
    t_json_observation_unserializer unserializer(collector);
    unserializer(tree);

    t_count_spectra res;

    t_observations_to_count_spectra converter;
    BOOST_FOREACH(t_transaction_observation::t_ptr tr, 
                  collector->transactions){
      tr->flatten();
      converter(*tr, res);
    }
    std::cout << res;
  }
  catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    throw e;
  }

  return 0;
}
