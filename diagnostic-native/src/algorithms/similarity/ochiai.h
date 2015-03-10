#ifndef __OCHIAI_H_d6384c39acff6e3bd421299a3e55cf4c4df5e047__
#define __OCHIAI_H_d6384c39acff6e3bd421299a3e55cf4c4df5e047__

#include "../similarity.h"

namespace diagnostic {
namespace algorithms {
class t_ochiai : public t_similarity {
public:
    inline virtual std::string get_similarity_type () const {
        return "ochiai";
    }

protected:
    virtual t_score similarity_coefficient (const t_count n[2][2]) const;

    virtual std::ostream & write (std::ostream & out) const;
};
}
}
#endif