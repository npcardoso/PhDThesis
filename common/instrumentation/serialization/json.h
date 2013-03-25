#ifndef __COMMON_INSTRUMENTATION_SERIALIZATION_JSON_H__
#define __COMMON_INSTRUMENTATION_SERIALIZATION_JSON_H__

#include "instrumentation/serialization/serializer.h"

class t_json_serializer: public t_serializer{
  inline t_json_serializer() {}
public:
  static t_ptr instance();

  virtual std::ostream & observation(std::ostream & out,
                                     const t_oracle_observation & obs);

  virtual std::ostream & observation(std::ostream & out,
                                     const t_probe_observation & obs);

  virtual std::ostream & observation(std::ostream & out,
                                     const t_transaction_observation & obs);

  virtual std::ostream & observation_header(std::ostream & out);
  virtual std::ostream & observation_separator(std::ostream & out);
  virtual std::ostream & observation_footer(std::ostream & out);
  
  virtual std::ostream & observation_request_header(std::ostream & out);
  virtual std::ostream & observation_request_footer(std::ostream & out);
  
  
  virtual std::ostream & construct(std::ostream & out,
                                   const t_oracle_construct & ctr);

  virtual std::ostream & construct(std::ostream & out,
                                   const t_probe_construct & ctr);

  virtual std::ostream & construct(std::ostream & out,
                                   const t_transaction_construct & ctr);

  virtual std::ostream & construct_header(std::ostream & out);
  virtual std::ostream & construct_separator(std::ostream & out);
  virtual std::ostream & construct_footer(std::ostream & out);

  virtual std::ostream & construct_request_header(std::ostream & out);
  virtual std::ostream & construct_request_footer(std::ostream & out);
  
  virtual std::ostream & request_separator(std::ostream & out);
};

#endif
