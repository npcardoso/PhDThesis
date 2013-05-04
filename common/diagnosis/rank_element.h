#ifndef __DIAGNOSIS_RANK_ELEMENT_H__
#define __DIAGNOSIS_RANK_ELEMENT_H__

#include "types.h"

namespace diagnosis {

class t_rank_element {
public:
  typedef float t_rank_score;

  t_rank_element(): component(0), value(0){
  }
  
  t_rank_element(t_component_id component, t_rank_score value): component(component), value(value){
  }
  
  const t_component_id & get_component() const {
    return component;
  }
  
  const t_rank_score & get_value() const {
    return value;
  }

  bool operator < (const t_rank_element & elem) const {
    return value > elem.value;
  }

private:
  t_component_id component;
  t_rank_score value;
};

}
#endif
