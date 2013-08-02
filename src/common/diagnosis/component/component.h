#ifndef __COMMON_DIAGNOSIS_COMPONENT_COMPONENT_H__
#define __COMMON_DIAGNOSIS_COMPONENT_COMPONENT_H__

#include "goodness.h"
#include "types.h"

#include <map>

namespace diagnosis {
namespace component {
class t_component {
public:
    t_component (t_probability_model::t_ptr g_model);

    const t_goodness_model& goodness_model () const;

    const char*metadata (std::string key) const;
    void metadata (std::string key, std::string value);

private:
    t_probability_model::t_ptr g_model;

    typedef std::map < std::string, std::string >t_metadata;
    t_metadata mdata;
};
}
}
#endif