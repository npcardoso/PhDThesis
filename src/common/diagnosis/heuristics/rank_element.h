#ifndef __RANK_ELEMENT_H_35f40c4aa871c5c9f111025d99c13629288805d7__
#define __RANK_ELEMENT_H_35f40c4aa871c5c9f111025d99c13629288805d7__

#include "types.h"

#include <iostream>
namespace diagnosis {
class t_rank_element {
public:
    typedef double t_score;
    typedef enum {NORMAL, SKIP, STOP} t_action;

    t_rank_element () {}
    t_rank_element (t_component_id component,
                    t_score score,
                    t_action action=NORMAL);

    t_action get_action () const;
    t_component_id get_component () const;
    t_score get_score () const;

    bool operator < (const t_rank_element & elem) const;

private:
    t_action action;
    t_component_id component;
    t_score score;
};
}

namespace std {
ostream & operator << (ostream & out, const diagnosis::t_rank_element & re);
}

#endif