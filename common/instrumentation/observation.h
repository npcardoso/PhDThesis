#ifndef __COMMON_INSTRUMENTATION_OBSERVATION_H__
#define __COMMON_INSTRUMENTATION_OBSERVATION_H__

#include "types.h"
#include "utils/boost.h"
#include "utils/time.h"

namespace instrumentation {
class t_observation {
public:
  DEFINE_BOOST_SHARED_PTRS(t_observation);

  virtual size_t size() const = 0;

  virtual bool operator > (const t_time_interval & time) const = 0;
  virtual bool operator < (const t_time_interval & time) const = 0;
};

class t_observation_single: public t_observation {
protected:
  t_observation_single();
  t_observation_single(t_time_interval time, 
                       t_construct_id c_id);

public:
  virtual bool operator > (const t_time_interval & time) const;
  virtual bool operator < (const t_time_interval & time) const;
  
  t_time_interval time;
  t_construct_id c_id; 
};

class t_observation_window: public t_observation {
protected:
  t_observation_window();
  t_observation_window(t_time_interval time, 
                       t_construct_id c_id);
  t_observation_window(t_time_interval time_start, 
                       t_construct_id c_id_start,
                       t_time_interval time_end, 
                       t_construct_id c_id_end);
public:
  virtual bool ended() const;

  virtual bool operator > (const t_time_interval & time) const;
  virtual bool operator < (const t_time_interval & time) const;
  
  t_time_interval time_start, time_end;
  t_construct_id c_id_start, c_id_end; 
};
}

#endif

