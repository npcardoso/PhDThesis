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

t_diagnosis_report::t_diagnosis_report () {
    total_prob = 0;
}

t_diagnosis_report::t_diagnosis_report (const t_diagnosis_report::t_D & D,
                                        const t_diagnosis_report::t_probs & probs) {
    total_prob = 0;
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
    assert(i <= D.size());
    return *rank[i - 1].get_element();
}

const t_diagnosis_report::t_probs::value_type & t_diagnosis_report::get_probability (t_id i) const {
    assert(i > 0);
    assert(i <= probs.size());
    return *rank[i - 1].get_score();
}

const t_diagnosis_report::t_probs::value_type & t_diagnosis_report::get_probability () const {
    return total_prob;
}

t_count t_diagnosis_report::size () const {
    assert(D.size() == probs.size());
    return D.size();
}

void t_diagnosis_report::add (const t_D::value_type & candidate,
                              const t_probs::value_type & prob) {
    D.push_back(candidate);
    probs.push_back(prob);
    t_rank_element tmp(&(D.back()), &(probs.back()));
    rank.push_back(tmp);
    total_prob += probs.back();

    sort(rank.begin(), rank.end(), t_diagnosis_report::compare);
}

void t_diagnosis_report::add (const t_D & candidates,
                              const t_probs & prob_list) {
    assert(candidates.size() == prob_list.size());
    t_D::const_iterator it = candidates.begin();
    t_probs::const_iterator it_probs = prob_list.begin();

    for (t_id i = 0; i < candidates.size(); i++) {
        D.push_back(*(it++));
        probs.push_back(*(it_probs++));
        t_rank_element tmp(&(D.back()), &(probs.back()));
        rank.push_back(tmp);
        total_prob += probs.back();
    }

    sort(rank.begin(), rank.end(), t_diagnosis_report::compare);
}

bool t_diagnosis_report::compare (const t_rank_element & r1, const t_rank_element & r2) {
    return *r1.get_score() > *r2.get_score(); // Larger first
}
}
}

namespace std {
std::ostream & operator << (std::ostream & out, const diagnosis::structs::t_diagnosis_report & dr) {
    for (t_id i = 1; i <= dr.size(); i++)
        out << (dr.get_probability(i) / dr.get_probability()) << ": " << dr.get_candidate(i) << std::endl;

    return out;
}
}