#ifndef __TARANTULA_H_28db782c2dbbc332e0b40bc2ca9d4be25a22a22d__
#define __TARANTULA_H_28db782c2dbbc332e0b40bc2ca9d4be25a22a22d__

#include "../similarity.h"

namespace diagnostic {
namespace algorithms {
class t_tarantula : public t_similarity {
public:
    inline virtual std::string get_similarity_type () const {
        return "tarantula";
    }

protected:
    virtual t_score similarity_coefficient (const t_count n[2][2]) const;

    virtual std::ostream & write (std::ostream & out) const;
};
}
}
#endif