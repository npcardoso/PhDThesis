#ifndef __DIAGNOSIS_STRUCTS_CANDIDATE_H__
#define __DIAGNOSIS_STRUCTS_CANDIDATE_H__

#include "types.h"

#include <iostream>
#include <set>

namespace diagnosis {
namespace structs {
class t_candidate : public std::set<t_component_id> {
public:
    inline t_candidate () {}
    template <class InputIterator>
    t_candidate (InputIterator first, InputIterator last,
                 const key_compare & comp=key_compare(),
                 const allocator_type & alloc=allocator_type()) : set<t_component_id> (first, last, comp, alloc) {}

    virtual std::istream & read (std::istream & in);
    virtual std::ostream & print (std::ostream & out) const;
};
}
}
namespace std {
inline std::istream & operator >> (std::istream & in, diagnosis::structs::t_candidate & candidate) {
    return candidate.read(in);
}

inline std::ostream & operator << (std::ostream & out, const diagnosis::structs::t_candidate & candidate) {
    return candidate.print(out);
}
}
#endif