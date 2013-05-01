#include "utils/debug.h"
#include <iostream>
#include <string>
#include "serialization/unserializers/json.h"
#include "serialization/serializers/json.h"
#include <boost/property_tree/json_parser.hpp>

#define BUF_SIZE 102400

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

    t_observation_sink::t_ptr cerr_sink(new t_json_observation_serializer(std::cerr, t_element_group_writer::t_ptr(new t_json_array())));
    t_json_observation_unserializer unserializer(cerr_sink);
  
    unserializer(tree);
  }
  catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    throw e;
  }

  return 0;
}
