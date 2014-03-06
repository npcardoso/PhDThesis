#ifndef __RANK_ELEMENT_H_35f40c4aa871c5c9f111025d99c13629288805d7__
#define __RANK_ELEMENT_H_35f40c4aa871c5c9f111025d99c13629288805d7__

#include <iostream>

namespace diagnosis {
template <class C, class S>
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

    bool operator < (const t_rank_element<C, S> & elem) const {
        return score > elem.score;
    }

private:
    t_element element;
    t_score score;
};
}

namespace std {
template <class C, class S>
ostream & operator << (ostream & out, const diagnosis::t_rank_element<C, S> & re) {
    return out << "t_rank_element(" << re.get_element() << "," << re.get_score() << ")";
}
}

#endif