#include "single_component_ranking.h"

#include <boost/foreach.hpp>

#include <cassert>
#include <algorithm>

namespace diagnostic {
t_single_component_ranking::t_single_component_ranking (t_count size) : rank(size) {}

void t_single_component_ranking::set (t_id i,
                                      t_score s,
                                      t_component_id c) {
    assert(i < rank.size());
    rank[i] = t_ranking_element(s, c);
}

void t_single_component_ranking::sort () {
    std::sort(rank.begin(), rank.end(), std::greater<t_ranking_element> ());
}

void t_single_component_ranking::normalize () {
    t_score total = 0;


    BOOST_FOREACH(const auto & e,
                  rank) {
        total += e.first;
    }

    BOOST_FOREACH(auto & e,
                  rank) {
        e.first /= total;
    }
}

t_component_id t_single_component_ranking::get_component (t_id i) const {
    assert(i < rank.size());
    return rank[i].second;
}

t_score t_single_component_ranking::get_score (t_id i) const {
    assert(i < rank.size());
    return rank[i].first;
}

t_count t_single_component_ranking::size () const {
    return rank.size();
}
}

namespace std {
ostream & operator << (ostream & out,
                       const diagnostic::t_single_component_ranking & r) {
    for (t_id i = 0; i < r.size(); i++)
        out << r.get_score(i) << ", " << r.get_component(i) << "\n";

    return out;
}
}