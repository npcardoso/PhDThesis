#include "component.h"

namespace diagnosis {
namespace component {

t_component::t_component(t_goodness_model * g_model):g_model(g_model) {

}

t_component::t_component(boost::shared_ptr<t_goodness_model> g_model):g_model(g_model) {

}

const t_goodness_model & t_component::goodness_model() const {
  return *g_model;
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
