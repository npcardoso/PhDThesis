#ifndef __INSTR_CONSTRUCT_H__
#define __INSTR_CONSTRUCT_H__

#include "types.h"

#include <boost/shared_ptr.hpp>

#include <map>
#include <string>

class t_construct {
  typedef std::map<std::string, std::string> t_metadata_storage;
  t_metadata_storage _metadata;
  // TODO: construct_type
  
public:
  typedef char t_type;
  typedef boost::shared_ptr<t_construct> t_ptr;
  typedef boost::shared_ptr<const t_construct> t_const_ptr;

  void metadata(t_construct_id o_id, 
                std::string key, 
                std::string value);

  t_type type() const;
  
};

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
  
  inline t_observation_single(t_time_interval time, 
                              t_construct_id c_id) : time(time), c_id(c_id) {}
  
  inline virtual bool operator > (const t_time_interval & time) const {
    return this->time > time;
  }
  
  inline virtual bool operator < (const t_time_interval & time) const {
    return this->time < time;
  }
};

class t_observation_window: public t_observation {
public:
  t_time_interval time_start, time_end;
  t_construct_id c_id_start, c_id_end; 
  
  inline t_observation_window(t_time_interval time, 
                              t_construct_id c_id) {
    this->time_start = time;
    this->time_end = 0;
    this->c_id_start = c_id;
    this->c_id_end = 0;
  }
  
  inline bool ended() const {
    return time_start < time_end || c_id_end;
  }
  
  inline virtual bool operator > (const t_time_interval & time) const {
    return this->time_start > time;
  }
  
  inline virtual bool operator < (const t_time_interval & time) const {
    return this->time_end < time;
  }
};
#endif
