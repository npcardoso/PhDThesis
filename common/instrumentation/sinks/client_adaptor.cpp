#include "client_adaptor.h"

#include <sstream>

namespace instrumentation {
namespace sinks {

t_client_adaptor::t_client_adaptor(t_client::t_ptr client,
                                   t_observation_serializer::t_ptr serializer):client(client), serializer(serializer) {
}

bool t_client_adaptor::operator << (const t_transaction_observation::t_ptr & obs) {
  std::stringstream buf;
  (*serializer)(buf, obs);
  (*serializer).close(buf);
  (*client) << buf.str();
  return true;
}

bool t_client_adaptor::operator << (const t_oracle_observation::t_ptr & obs) {
  std::stringstream buf;
  (*serializer)(buf, obs);
  (*serializer).close(buf);
  (*client) << buf.str();
  return true;
}

bool t_client_adaptor::operator << (const t_probe_observation::t_ptr & obs) {
  std::stringstream buf;
  (*serializer)(buf, obs);
  (*serializer).close(buf);
  (*client) << buf.str();
  return true;
}

}
}
