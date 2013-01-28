#include "candidate.h"

std::istream & operator >> (std::istream & in, t_candidate & candidate) {
  t_component_id component;
  candidate.clear();
//FIXME: Change this condition
  while (true) {
    in >> component;
    if(component == 0)
      break;
    candidate.insert(component);
  }
  return in;
}
