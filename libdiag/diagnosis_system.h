#ifndef __DIAGNOSIS_SYSTEM_H_2d027fe833f058443bd53e5b08fdd5e63b74e055__
#define __DIAGNOSIS_SYSTEM_H_2d027fe833f058443bd53e5b08fdd5e63b74e055__

#include "types.h"
#include "structs/spectra.h"
#include "structs/spectra_filter.h"
#include "structs/trie.h"
// #include "utils/mpreal.h"
#include "utils/boost.h"

#include <list>
#include <ostream>

namespace diagnosis {
class t_candidate_generator {
public:
    typedef structs::t_trie t_ret_type;

    virtual void operator () (const structs::t_spectra & spectra,
                              t_ret_type & D,
                              const structs::t_spectra_filter * filter=NULL) const = 0;
    virtual std::string to_string() const = 0;
    inline virtual ~t_candidate_generator () {}
};


class t_candidate_ranker {
public:
    class t_ret_type : public std::list<t_probability_mp> {
public:
    };

    virtual void operator () (const structs::t_spectra & spectra,
                              const structs::t_trie & D,
                              t_ret_type & probs,
                              const structs::t_spectra_filter * filter=NULL) const = 0;
    virtual std::string to_string() const = 0;
    inline virtual ~t_candidate_ranker () {}
};


}

namespace std {
std::ostream& operator<<(std::ostream & s,
                         const diagnosis::t_candidate_generator & c);
std::ostream& operator<<(std::ostream & s,
                         const diagnosis::t_candidate_ranker & c);
}

#endif
