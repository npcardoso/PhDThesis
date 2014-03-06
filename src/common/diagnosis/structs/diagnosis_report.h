#ifndef __DIAGNOSIS_REPORT_H_132a205ec90bbf9374e48a44e208119ba72231fa__
#define __DIAGNOSIS_REPORT_H_132a205ec90bbf9374e48a44e208119ba72231fa__

#include "diagnosis/diagnosis_system.h"


namespace diagnosis {
namespace structs {
class t_diagnosis_report {
public:
    typedef t_candidate_generator::t_ret_type t_D;
    typedef t_candidate_ranker::t_ret_type t_probs;
    typedef t_probs::value_type t_entropy;

    t_diagnosis_report (const t_D & D,
                        const t_probs & probs);

    t_id next (t_id current,
               const t_candidate & healthy_comps) const;

    const t_D::value_type & get_candidate (t_id i) const;
    double get_probability (t_id i) const;

    t_count size () const;

    t_entropy get_entropy () const;

private:
    void add (const t_D & D,
              const t_probs & probs);


    typedef std::list<t_D::value_type> t_candidate_list;
    t_candidate_list D;

    typedef std::pair<double, t_D::value_type *> t_rank_element;
    std::vector<t_rank_element> rank;
};
}
}

namespace std {
std::ostream & operator << (std::ostream & out, const diagnosis::structs::t_diagnosis_report & dr);
}

#endif