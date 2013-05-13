#include "client/client.h"

t_client & t_client::operator << (const std::stringstream & s) {
  return ((*this) << s.str());
}

