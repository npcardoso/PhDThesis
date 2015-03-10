#ifndef __SINGLE_COMPONENT_RANKING_H_840a6b2b9143ccfd0b818a79b9cb797d5ca07a72__
#define __SINGLE_COMPONENT_RANKING_H_840a6b2b9143ccfd0b818a79b9cb797d5ca07a72__

#include "../types.h"

#include <iostream>
#include <vector>

namespace diagnostic {
class t_single_component_ranking {
public:

    t_single_component_ranking (t_count size);

    void set (t_id i,
              t_score s,
              t_component_id c);

    void sort ();
    void normalize ();

    t_component_id get_component (t_id i) const;
    t_score get_score (t_id i) const;
    t_count size () const;


private:
    typedef std::pair<t_score, t_component_id> t_ranking_element;
    std::vector<t_ranking_element> rank;
};
}

namespace std {
ostream & operator << (ostream & out,
                       const diagnostic::t_single_component_ranking &);
}

#endif