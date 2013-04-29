#ifndef __COMMON_INSTRUMENTATION_SERIALIZATION_JSON_H__
#define __COMMON_INSTRUMENTATION_SERIALIZATION_JSON_H__

#include "serialization/serializer.h"
class t_element_group {
protected:
  bool first;
public:
  typedef boost::shared_ptr<t_element_group> t_ptr;
  typedef boost::shared_ptr<const t_element_group> t_const_ptr;
  
  inline t_element_group() {
    first = true;
  }
  
  virtual inline bool empty() const {
    return first;
  }
  
  virtual inline std::ostream & put(std::ostream & out) {
    first = false;
    return out;
  }
  
  virtual inline void close(std::ostream & out) {
    first = true;
  }
};

class t_json_array: public t_element_group {
public:
  typedef boost::shared_ptr<t_element_group> t_ptr;
  typedef boost::shared_ptr<const t_element_group> t_const_ptr;
  
  virtual std::ostream & put(std::ostream & out);
  virtual void close(std::ostream & out);
};

class t_json_map: public t_element_group {
public:
  typedef boost::shared_ptr<t_element_group> t_ptr;
  typedef boost::shared_ptr<const t_element_group> t_const_ptr;
  
  virtual std::ostream & put(std::ostream & out);
  virtual void close(std::ostream & out);
};

class t_json_observation_serializer: public t_observation_serializer {
public:
  t_json_observation_serializer(std::ostream & out);
  t_json_observation_serializer(std::ostream & out, t_element_group::t_ptr group);
  
  virtual bool operator << (const t_transaction_observation::t_ptr & obs);
  virtual bool operator << (const t_oracle_observation::t_ptr & obs);
  virtual bool operator << (const t_probe_observation::t_ptr & obs);
  
  virtual t_ptr array();
  virtual ~t_json_observation_serializer();
  
  virtual void close();

private:
  std::ostream & out;
  
  t_element_group::t_ptr group;
  std::ostream & observation_single(const t_observation_single & obs);
  std::ostream & observation_window(const t_observation_window & obs);
  std::ostream & timestamp(t_time_interval t);
  std::ostream & string(std::string s);
  std::ostream & key(std::string k);
};

class t_json_construct_serializer: public t_construct_serializer {
  
  virtual bool operator << (const t_transaction_construct::t_ptr & obs);
  virtual bool operator << (const t_oracle_construct::t_ptr & obs);
  virtual bool operator << (const t_probe_construct::t_ptr & obs);
  
  virtual t_ptr array() ;
  virtual ~t_json_construct_serializer();
  
  virtual void close();
};

#endif
