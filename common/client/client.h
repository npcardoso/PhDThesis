#ifndef __COMMON_CLIENT_CLIENT_H__
#define __COMMON_CLIENT_CLIENT_H__

#include "utils/boost.h"

#include <string>
#include <sstream>

class t_client {
public:
  DEFINE_BOOST_SHARED_PTRS(t_client);

  virtual t_client & operator << (const std::stringstream & s);
  virtual t_client & operator << (const std::string & s) = 0;
};

#endif
