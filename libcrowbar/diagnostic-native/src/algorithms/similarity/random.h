#ifndef __RANDOM_H_3decb83dd1653dc23a3d708618f3f228a05cd9b4__
#define __RANDOM_H_3decb83dd1653dc23a3d708618f3f228a05cd9b4__

#include "../similarity.h"

namespace diagnostic {
namespace algorithms {
class t_random : public t_similarity {
public:
    inline virtual std::string get_similarity_type () const {
        return "random";
    }

    virtual t_score operator () (const t_spectrum & spectrum,
                                 t_component_id comp,
                                 const t_spectrum_filter * filter=NULL) const;

protected:
    inline virtual t_score similarity_coefficient (const t_count n[2][2]) const {
        return 0;
    }
};
}
}
#endif