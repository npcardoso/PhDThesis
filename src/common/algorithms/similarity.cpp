#include "similarity.h"

#include <boost/foreach.hpp>

#include <cstring>
#include <cmath>

#include <random>

#include <algorithm>
#include <functional>
#include <boost/random/uniform_real_distribution.hpp>

namespace diagnosis {
namespace algorithms {
using namespace structs;

t_rank::t_rank (t_count size) : rank(size) {}

void t_rank::set (t_id i,
                  t_score s,
                  t_component_id c) {
    assert(i < rank.size());
    rank[i] = t_rank_element(s, c);
}

void t_rank::sort () {
    std::sort(rank.begin(), rank.end(), std::greater<t_rank_element> ());
}

void t_rank::normalize () {
    t_score total = 0;


    BOOST_FOREACH(t_rank_element & e,
                  rank) {
        total += e.first;
    }

    BOOST_FOREACH(t_rank_element & e,
                  rank) {
        e.first /= total;
    }
}

t_component_id t_rank::get_component (t_id i) const {
    assert(i < rank.size());
    return rank[i].second;
}

t_score t_rank::get_score (t_id i) const {
    assert(i < rank.size());
    return rank[i].first;
}

t_count t_rank::size () const {
    return rank.size();
}

t_ptr<t_rank> t_similarity::operator () (const structs::t_spectra & spectra,
                                         const structs::t_spectra_filter * filter) const {
    t_spectra_iterator it(spectra.get_component_count(),
                          spectra.get_transaction_count(),
                          filter);


    t_ptr<t_rank> rank(new t_rank(spectra.get_component_count(filter)));
    t_id i = 0;

    while (it.component.next()) {
        t_score score = (* this)(spectra,
                                 it.component.get(),
                                 filter);

        rank->set(i++, score, it.component.get());
    }

    return rank;
}

t_score t_similarity::operator () (const structs::t_spectra & spectra,
                                   t_component_id comp,
                                   const structs::t_spectra_filter * filter) const {
    t_spectra_iterator it(spectra.get_component_count(),
                          spectra.get_transaction_count(),
                          filter);

    t_count n[2][2];


    memset(n, 0, sizeof(t_count) * 4);

    while (it.transaction.next()) {
        bool activity = spectra.is_active(comp, it.transaction.get());
        bool error = spectra.is_error(it.transaction.get());
        n[activity ? 1 : 0][error ? 1 : 0]++;
    }

    assert(n[0][0] + n[0][1] + n[1][0] + n[1][1] == spectra.get_transaction_count(filter));
    return similarity_coefficient(n);
}

t_score t_ochiai::similarity_coefficient (const t_count n[2][2]) const {
    if (!n[1][1])
        return 0;

    t_score tmp = ((n[1][1] + n[0][1]) * (n[1][1] + n[1][0]));
    return n[1][1] / sqrt(tmp);
}

std::ostream & t_ochiai::print (std::ostream & out) const {
    return out << "t_ochiai";
}

t_score t_tarantula::similarity_coefficient (const t_count n[2][2]) const {
    if (!n[1][1])
        return 0;

    t_score tmp1 = n[1][1] / (t_score) (n[1][1] + n[0][1]);
    t_score tmp2 = 0;

    if (!n[1][0])
        tmp2 = n[1][0] / (t_score) (n[1][0] + n[0][0]);

    return tmp1 / (tmp1 + tmp2);
}

std::ostream & t_tarantula::print (std::ostream & out) const {
    return out << "t_tarantula";
}

t_score t_jaccard::similarity_coefficient (const t_count n[2][2]) const {
    if (!n[1][1])
        return 0;

    t_score tmp = (n[1][1] + n[0][1] + n[1][0]);
    return n[1][1] / tmp;
}

std::ostream & t_jaccard::print (std::ostream & out) const {
    return out << "t_jaccard";
}

t_score t_random::operator () (const structs::t_spectra & spectra,
                               t_component_id comp,
                               const structs::t_spectra_filter * filter) const {
    std::mt19937 gen;
    boost::random::uniform_real_distribution<t_error> rand(0, 1);


    return rand(gen);
}
}
}
namespace std {
ostream & operator << (ostream & out, const diagnosis::algorithms::t_rank & r) {
    for (t_id i = 0; i < r.size(); i++)
        out << r.get_score(i) << ", " << r.get_component(i) << "\n";

    return out;
}
}