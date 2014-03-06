#include "rank_element.h"
namespace diagnosis {
t_rank_element::t_rank_element (t_component_id component, t_score score, t_action action) {
    this->component = component;
    this->score = score;
    this->action = action;
}

t_rank_element::t_action t_rank_element::get_action () const {
    return action;
}

t_component_id t_rank_element::get_component () const {
    return component;
}

t_rank_element::t_score t_rank_element::get_score () const {
    return score;
}

bool t_rank_element::operator < (const t_rank_element & elem) const {
    return score > elem.score;
}
}

namespace std {
ostream & operator << (ostream & out, const diagnosis::t_rank_element & re) {
    return out << "t_rank_element(" <<
           re.get_score() << "," <<
           re.get_score() << "," <<
           re.get_action() << ")";
}
}