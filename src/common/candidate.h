#ifndef __CANDIDATE_H__
#define __CANDIDATE_H__

#include "types.h"

#include <iostream>
#include <set>

typedef std::set<t_component_id> t_candidate;

std::istream & operator >> (std::istream & in, t_candidate & candidate);


#endif
