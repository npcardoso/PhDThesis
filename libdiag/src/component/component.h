#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "goodness.h"
#include "../types.h"

#include <boost/shared_ptr.hpp>
#include <map>

class t_component {
private:
  boost::shared_ptr <t_goodness_model> g_model;
  
  typedef std::map <std::string, std::string> t_metadata;
  t_metadata mdata;

public:
  t_component(t_goodness_model * g_model);
  t_component(boost::shared_ptr<t_goodness_model> g_model);

  const t_goodness_model & goodness_model() const;

  const char * metadata(std::string key) const;
  void metadata(std::string key, std::string value);
};
#endif
