#ifndef __COMMON_IO_JSON_H__
#define __COMMON_IO_JSON_H__

#include <ostream>
#include "types.h"

#include "instrumentation/transaction.h"
#include "instrumentation/oracle.h"
#include "instrumentation/probe.h"

using namespace std;
class t_json {
public:
  static std::ostream & observation(std::ostream & out,
                                    const t_oracle_observation & obs);

  static std::ostream & observation(std::ostream & out,
                                    const t_probe_observation & obs);

  static std::ostream & observation(std::ostream & out,
                                    const t_transaction_observation & obs);

  static std::ostream & request_header(std::ostream & out,
                                       t_id id,
                                       char action);
};

#endif
