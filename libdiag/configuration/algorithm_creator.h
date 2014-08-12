#ifndef __ALGORITHM_CREATOR_H_30c5ed765030ca3048f32ed1499531b110412132__
#define __ALGORITHM_CREATOR_H_30c5ed765030ca3048f32ed1499531b110412132__

#include "../diagnostic_system.h"
#include "algorithm.h"

namespace diagnostic {
namespace configuration {
class t_algorithm_creator {
    public:
    class t_register {
        public:
        t_register(const t_const_ptr<t_algorithm_creator> & ac);
    };

    static const t_algorithm_creator & get_default();

    virtual t_const_ptr<t_candidate_generator> generator(const t_algorithm & a) const;

    virtual t_const_ptr<t_candidate_ranker> ranker(const t_algorithm & a) const;

    inline virtual ~t_algorithm_creator() {}

    protected:

    const std::string * get(const t_algorithm & a,
                            const std::string & key) const;
    inline t_algorithm_creator() {}
};
}
}

#endif
