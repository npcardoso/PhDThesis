#ifndef __TYPES_H__
#define __TYPES_H__

#include <set>

/* IDs */

typedef unsigned int t_id;
typedef t_id t_component_id;
typedef t_id t_transaction_id;
typedef t_id t_spectrum_id;

/* Counts */

typedef unsigned int t_count;
typedef t_count t_ns_count;

/* Ranking */

typedef float t_heuristic_value;

class t_rank_element {
  t_component_id component;
  t_heuristic_value value;
public:
  inline t_rank_element(): 
    component(0),
    value(0){
    }
  
  inline t_rank_element(t_component_id component,
                        t_heuristic_value value):
    component(component),
    value(value){
    }
  
  inline t_component_id get_component() const {
    return component;
  }
  
  inline t_heuristic_value get_value() const {
    return value;
  }

  inline bool operator < (const t_rank_element & elem) const {
    return value > elem.value;
  }

};

/* Candidate */

typedef std::set<t_component_id> t_candidate;

#endif
