#include "server/services/json.h"

#include "utils/debug.h"

#include <boost/property_tree/json_parser.hpp>


using boost::property_tree::ptree;

void json_copy_object(std::istream & in, std::ostream & out) {
  bool in_str = false;
  bool escape = false;
  t_count stack_size = 0;

  char c;
  while(in >> c) {

    if(escape) {
      escape = false;
    }
    else if(c == '"') {
      in_str = !in_str;
    }
    else if(in_str){
      escape = c == '\\';
    }
    else if(c == '[' || c == '{'){
      stack_size++;
    }
    else if(c == ']' || c == '}'){
      stack_size--;
      if(!stack_size) {
        out << c;
        break;
      }
    }
    
    if(stack_size && (in_str || isprint(c)))
      out << c;
  }
}

t_json_adapter_service::t_json_adapter_service(const t_json_service::t_ptr & srv):srv(srv) {
}

void t_json_adapter_service::operator ()(std::istream & in,
                                         std::ostream & out) {

  while(in) {
    boost::property_tree::ptree pt;
    std::stringstream object;
    json_copy_object(in, object);
    if(!object.str().size())
      continue;
    try{
      boost::property_tree::read_json(object, pt);
      debug("Parsing Complete");
      (*srv)(in, out, pt);
    }
    catch (std::exception& e) {
      debug("Parse Error");
      std::cerr << e.what() << std::endl;
    }
  }
  debug("Ended Connection");
}
