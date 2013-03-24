#ifndef __COMMON_IO_JSON_H__
#define __COMMON_IO_JSON_H__

#include <ostream>
#include "types.h"
#include "datastore/datastore.h"
#include "datastore/oracle.h"
#include "datastore/transaction.h"
#include "datastore/probe.h"

#include <boost/property_tree/ptree.hpp>

using namespace std;
namespace io {
namespace json {
  std::ostream & observation(std::ostream & out,
                             const t_oracle_observation & obs);

  std::ostream & observation(std::ostream & out,
                             const t_probe_observation & obs);

  std::ostream & observation(std::ostream & out,
                             const t_transaction_observation & obs);

  std::ostream & request_header(std::ostream & out,
                                t_id id,
                                char action);
}
}

#endif
