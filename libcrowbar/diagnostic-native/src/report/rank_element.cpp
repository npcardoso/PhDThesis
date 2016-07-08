#include "rank_element.h"

namespace diagnostic {

t_rank_element::t_rank_element(const t_candidate & candidate,
                               double score) {
    this->candidate = &candidate;
    this->score = score;
}

t_rank_element::t_rank_element(const t_rank_element & element) {
    *this = element;
}


double t_rank_element::get_score() const {
    return score;
}

const t_candidate & t_rank_element::get_candidate() const {
    return *candidate;
}

bool t_rank_element::operator > (const t_rank_element & e) const {
    return score < e.score;
}

t_rank_element & t_rank_element ::operator=(const t_rank_element  &rhs) {
    // Check for self-assignment!
    if (this == &rhs)
        return *this;

    candidate = rhs.candidate;
    score = rhs.score;

    return *this;
}


}


namespace std {
std::ostream & operator << (std::ostream & out,
                            const diagnostic::t_rank_element& e) {
        out << e.get_score() << ": " << e.get_candidate();

    return out;
}
}
