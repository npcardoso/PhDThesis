#ifndef __COMMON_DATASTORE_CONSTRUCT_H__
#define __COMMON_DATASTORE_CONSTRUCT_H__

#include "types.h"

#include <boost/shared_ptr.hpp>

#include <map>
#include <string>

class t_construct {
  typedef std::map<std::string, std::string> t_metadata_storage;
  t_metadata_storage _metadata;
  
public:
  typedef boost::shared_ptr<t_construct> t_ptr;
  typedef boost::shared_ptr<const t_construct> t_const_ptr;

  void metadata(t_construct_id o_id, 
                std::string key, 
                std::string value);

};

class t_construct_sink {
public:
  typedef boost::shared_ptr<t_construct_sink> t_ptr;
  typedef boost::shared_ptr<const t_construct_sink> t_const_ptr;

  virtual t_construct_id operator()(const t_construct::t_ptr & constr) = 0;
};


#endif
