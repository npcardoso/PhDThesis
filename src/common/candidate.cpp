#include "candidate.h"

std::istream & t_candidate::read(std::istream & in) {
  t_component_id component;
  while (true) {
    in >> component;
    if(component == 0 || !in.good())
      break;
    insert(component);
  }
  return in;
}

std::ostream & t_candidate::print(std::ostream & out) const {
    iterator component = begin();
    if(component != end())
      out << *(component++);

    while(component != end())
      out << "," << *(component++);
    return out;
}
