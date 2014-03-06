#include "diagnosis_report.h"

#include <boost/foreach.hpp>

namespace diagnosis {
namespace structs {
template <class InputIterator1, class InputIterator2>
bool intersects (InputIterator1 first1, InputIterator1 last1,
                 InputIterator2 first2, InputIterator2 last2) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) ++first1;
        else if (*first2 < *first1) ++first2;
        else
            return true;
    }

    return false;
}

t_diagnosis_report::t_diagnosis_report (const t_diagnosis_report::t_D & D,
                                        const t_diagnosis_report::t_probs & probs) {
    add(D, probs);
}

t_id t_diagnosis_report::next (t_id current,
                               const t_candidate & healthy_comps) const {
    while (current++ < rank.size()) {
        if (!intersects(healthy_comps.begin(), healthy_comps.end(),
                        get_candidate(current).begin(), get_candidate(current).end()))
            return current;
    }

    return 0;
}

const t_diagnosis_report::t_D::value_type & t_diagnosis_report::get_candidate (t_id i) const {
    assert(i > 0);
    assert(i <= rank.size());
    return *(rank[i - 1].second);
}

double t_diagnosis_report::get_probability (t_id i) const {
    assert(i > 0);
    assert(i <= rank.size());
    return rank[i - 1].first;
}

t_count t_diagnosis_report::size () const {
    return D.size();
}

void t_diagnosis_report::add (const t_D & candidates,
                              const t_probs & probs) {
    assert(candidates.size() == probs.size());

    t_probs::value_type total_prob = 0;
    total_prob = accumulate(probs.begin(), probs.end(), total_prob);

    t_D::const_iterator it = candidates.begin();

    BOOST_FOREACH(const t_probs::value_type & it_probs, probs) {
        D.push_back(*(it++));
        t_rank_element tmp((it_probs / total_prob).toDouble(),
                           &(D.back()));
        rank.push_back(tmp);
    }

    sort(rank.begin(), rank.end(), std::greater<t_rank_element> ());
}

t_diagnosis_report::t_entropy t_diagnosis_report::get_entropy () const {
    t_entropy ret = 0;


    for (t_id i = 1; i <= size(); i++) {
        double prob = get_probability(i);

        if (prob > 0)
            ret -= prob * log(prob);
    }

    return ret;
}
}
}
namespace std {
std::ostream & operator << (std::ostream & out, const diagnosis::structs::t_diagnosis_report & dr) {
    for (t_id i = 1; i <= dr.size(); i++)
        out << (dr.get_probability(i)) << ": " << dr.get_candidate(i) << std::endl;

    return out;
}
}