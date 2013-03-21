#ifndef __JSON_H__
#define __JSON_H__

#include "service.h"

#include <cctype>
#include <boost/property_tree/ptree.hpp>
#include <boost/shared_ptr.hpp> 

typedef boost::shared_ptr<boost::property_tree::ptree> t_ptree_ptr;

class t_json_service {
public:
  typedef boost::shared_ptr<t_json_service> t_ptr;

  virtual void operator ()(std::iostream & stream, boost::property_tree::ptree & pt) = 0;
};

class t_json_parser_service: public t_service {
  t_json_service::t_ptr srv;
public:
  t_json_parser_service(const t_json_service::t_ptr & srv);
  virtual void operator ()(std::iostream & stream);
};

class t_json_debug : public t_json_service {
public:
  virtual void operator ()(std::iostream & stream, boost::property_tree::ptree & pt);
};

#endif
