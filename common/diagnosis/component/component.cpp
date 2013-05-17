#include "component.h"

namespace diagnosis {
namespace component {

t_component::t_component(t_probability_model::t_ptr g_model):g_model(g_model) {

}

void t_component::metadata(std::string key, std::string value) {
  mdata[key] = value; 
}

const char * t_component::metadata(std::string key) const {
  t_metadata::const_iterator pos = mdata.find(key);
  if(pos == mdata.end())
    return NULL;
  return pos->second.c_str();
}

}
}
