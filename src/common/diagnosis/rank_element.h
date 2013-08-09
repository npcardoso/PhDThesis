#ifndef __DIAGNOSIS_RANK_ELEMENT_H__
#define __DIAGNOSIS_RANK_ELEMENT_H__

#include "types.h"

namespace diagnosis {
template <class S, class C>
class t_rank_element {
public:
    typedef S t_score;
    typedef C t_element;

    t_rank_element () {}

    t_rank_element (t_element element, t_score score) : element(element), score(score) {}

    const t_element & get_element () const {
        return element;
    }

    const t_score & get_score () const {
        return score;
    }

    bool operator < (const t_rank_element<S, C> & elem) const {
        return score > elem.score;
    }

private:
    t_element element;
    t_score score;
};
}
#endif