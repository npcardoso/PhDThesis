#ifndef __CANDIDATE_H__
#define __CANDIDATE_H__

#include "types.h"

#include <iostream>
#include <set>

class t_candidate: public std::set<t_component_id> {
public:
  virtual std::istream & read(std::istream & in);
  virtual std::ostream & print(std::ostream & out) const;
};

inline std::istream & operator >> (std::istream & in, t_candidate & candidate) {
  return candidate.read(in);
}

inline std::ostream & operator << (std::ostream & out, const t_candidate & candidate) {
  return candidate.print(out);
}


#endif
