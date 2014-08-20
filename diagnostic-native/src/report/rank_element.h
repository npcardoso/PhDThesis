#ifndef __RANK_ELEMENT_H_54b52ebbc8e937e3e0fc976d278ff055f14a7d68__
#define __RANK_ELEMENT_H_54b52ebbc8e937e3e0fc976d278ff055f14a7d68__

#include "../candidate.h"


namespace diagnostic {

class t_rank_element {
    public:
    /**
     * @brief: Creates a new ranking element.
     *
     * The candidate is not copied, so this object is only valid as
     * long as the backing candidate object is also valid.
     */
    t_rank_element(const t_candidate & candidate,
                   double score);

    t_rank_element(const t_rank_element & element);

    double get_score() const;
    const t_candidate & get_candidate() const;

    bool operator > (const t_rank_element & e) const;
    t_rank_element & operator=(const t_rank_element  &rhs);

    private:
    double score;
    const t_candidate * candidate;
};

}

namespace std {
std::ostream & operator << (std::ostream & out,
                            const diagnostic::t_rank_element& e);
}
#endif
