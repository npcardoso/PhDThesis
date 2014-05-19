#ifndef __CANDIDATE_H_edf867e97cb99b12f0025a979426b18b2db6b361__
#define __CANDIDATE_H_edf867e97cb99b12f0025a979426b18b2db6b361__

#include "types.h"
#include "utils/boost.h"

#include <iostream>
#include <set>

namespace diagnosis {
namespace structs {
#define CANDIDATE_NORMAL_PREFIX ""
#define CANDIDATE_NORMAL_SUFFIX " 0"
#define CANDIDATE_NORMAL_SEP " "
#define CANDIDATE_PRETTY_PREFIX "{c"
#define CANDIDATE_PRETTY_SUFFIX "}"
#define CANDIDATE_PRETTY_SEP ", c"
#define CANDIDATE_LATEX_PREFIX "\\{c_{"
#define CANDIDATE_LATEX_SUFFIX "}\\}"
#define CANDIDATE_LATEX_SEP "}, c_{"


class t_candidate : public std::set<t_component_id> {
public:
    inline t_candidate () {}
    template <class InputIterator>
    t_candidate (InputIterator first, InputIterator last,
                 const key_compare & comp=key_compare(),
                 const allocator_type & alloc=allocator_type()) : set<t_component_id> (first, last, comp, alloc) {}

    virtual std::istream & read (std::istream & in);

    virtual std::ostream & generic_print (std::ostream & out,
                                          std::string prefix,
                                          std::string suffix,
                                          std::string separator) const;

    virtual inline std::ostream & print (std::ostream & out) const {
        return generic_print(out,
                             CANDIDATE_NORMAL_PREFIX,
                             CANDIDATE_NORMAL_SUFFIX,
                             CANDIDATE_NORMAL_SEP);
    }

    virtual inline std::ostream & pretty_print (std::ostream & out) const {
        return generic_print(out,
                             CANDIDATE_PRETTY_PREFIX,
                             CANDIDATE_PRETTY_SUFFIX,
                             CANDIDATE_PRETTY_SEP);
    }

    virtual inline std::ostream & latex_print (std::ostream & out) const {
        return generic_print(out,
                             CANDIDATE_LATEX_PREFIX,
                             CANDIDATE_LATEX_SUFFIX,
                             CANDIDATE_LATEX_SEP);
    }
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