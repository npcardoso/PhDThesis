#pragma once

#include "types.h"
#include "datastore/construct.h"

#include <boost/shared_ptr.hpp>

struct t_transaction_observation: public t_observation_window{

public:
  typedef boost::shared_ptr<t_transaction_observation> t_ptr;
  typedef boost::shared_ptr<const t_transaction_observation> t_const_ptr;

  inline t_transaction_observation(t_time_interval time,
                                   t_construct_id c_id): t_observation_window(time, c_id) {}
  
  virtual size_t size() const;
};
