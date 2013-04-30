#include "server/service.h"

void t_service::operator()(const t_iostream_ptr & in,
                           const t_iostream_ptr & out) {
  (*this)(*in, *out);
}

