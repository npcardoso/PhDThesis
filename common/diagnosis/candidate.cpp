#include "candidate.h"
namespace diagnosis {
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
    while(component != end())
      out << *(component++) << " ";
    return out << "0";
}
}
