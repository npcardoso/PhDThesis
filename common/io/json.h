#ifndef __COMMON_IO_JSON_H__
#define __COMMON_IO_JSON_H__

#include <ostream>
#include "types.h"
#include "datastore/observation_sink.h"

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

class t_json_observation_sink: public t_observation_sink {
  ostream & out;
public:
  t_json_observation_sink(ostream & out);

  virtual bool operator()(const t_transaction_observation::t_ptr & obs);
  virtual bool operator()(const t_oracle_observation::t_ptr & obs);
  virtual bool operator()(const t_probe_observation::t_ptr & obs);
};

#endif
