#ifndef __SERVER_SERVICES_JSON_H__
#define __SERVER_SERVICES_JSON_H__

#include "server/service.h"
#include "types.h"

#include <cctype>
#include <boost/property_tree/ptree.hpp>

class t_json_service {
public:
  DEFINE_BOOST_SHARED_PTRS(t_json_service);

  virtual void operator ()(std::istream & in,
                           std::ostream & out,
                           const boost::property_tree::ptree & pt) = 0;
};


class t_json_adapter_service: public t_service {
private:
  t_json_service::t_ptr srv;
public:
  t_json_adapter_service(const t_json_service::t_ptr & srv);

  virtual void operator ()(std::istream & in,
                           std::ostream & out);
};

#endif
