#ifndef __COMMON_DATASTORE_OBSERVATION_H__
#define __COMMON_DATASTORE_OBSERVATION_H__

#include "types.h"

#include <boost/shared_ptr.hpp>

class t_observation {
public:
  typedef boost::shared_ptr<t_observation> t_ptr;
  typedef boost::shared_ptr<const t_observation> t_const_ptr;

  virtual size_t size() const = 0;

  virtual bool operator > (const t_time_interval & time) const = 0;
  virtual bool operator < (const t_time_interval & time) const = 0;
};

class t_observation_single: public t_observation {
public:
  t_time_interval time;
  t_construct_id c_id; 

  t_observation_single(t_time_interval time, 
                       t_construct_id c_id);

  virtual bool operator > (const t_time_interval & time) const;
  virtual bool operator < (const t_time_interval & time) const;
};

class t_observation_window: public t_observation {
public:
  t_time_interval time_start, time_end;
  t_construct_id c_id_start, c_id_end; 

  t_observation_window(t_time_interval time, 
                       t_construct_id c_id);

  virtual bool ended() const;

  virtual bool operator > (const t_time_interval & time) const;
  virtual bool operator < (const t_time_interval & time) const;
};


class t_observation_sink {
public:
  typedef boost::shared_ptr<t_observation_sink> t_ptr;
  typedef boost::shared_ptr<const t_observation_sink> t_const_ptr;

  virtual void operator()(const t_observation::t_ptr & obs) = 0;
};

#endif

