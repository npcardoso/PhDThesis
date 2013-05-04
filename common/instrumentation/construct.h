#ifndef __COMMON_INSTRUMENTATION_CONSTRUCT_H__
#define __COMMON_INSTRUMENTATION_CONSTRUCT_H__

#include "types.h"

#include <boost/shared_ptr.hpp>

#include <map>
#include <string>
namespace instrumentation {
class t_construct {
public:
  typedef std::map<std::string, std::string> t_metadata_storage;
  t_metadata_storage _metadata;

  t_construct_id c_id;

  DEFINE_BOOST_SHARED_PTRS(t_construct);

  virtual void metadata(t_construct_id o_id, 
                        std::string key, 
                        std::string value);

};
}
#endif
