#include "types.h"
#include "utils/iostream.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <map>
#include <set>
#include <list>
#include <vector>
#include <string>
#include <boost/foreach.hpp>

#define BUF_SIZE 102400
using namespace std;
typedef enum {PASS = -1, UNKNOWN, FAIL = 1} t_outcome;

typedef std::pair<float, t_outcome> t_feedback_token;


class t_knn : public std::set<t_feedback_token> {
public:
    t_outcome operator () (float val, t_count k) const {
        const_iterator left = lower_bound(t_feedback_token(val, UNKNOWN));
        const_iterator right;


        if (left == end()) {
            right = end();
            left--;
        }
        else {
            right = left;
            right++;
        }

        if (k > size())
            return UNKNOWN;

        t_count fail_count = 0;
        t_count i = k;

        while (i--) {
            iterator tmp;

            if (left == end())
                tmp = right++;
            else if (right == end())
                tmp = left--;
            else if (abs(val - left->first) < abs(val - right->first)) {
                tmp = left;

                if (left == begin())
                    left = end();
                else
                    left--;
            }
            else
                tmp = right++;

            if (tmp->second)
                fail_count++;

            // cout << i << " -- "<< tmp->first << ": " << tmp->second << " fc: " << fail_count << " ret:" << (fail_count *2 >k)<<std::endl;
        }

        if (fail_count * 2 > k)
            return FAIL;
        else if (fail_count * 2 < k)
            return PASS;
        else
            return UNKNOWN;
    }
};


typedef pair<t_component_id, float> t_observation;
class t_transaction : public list<t_observation> {
public:
    t_outcome outcome;
};

void purge_empty (list<t_transaction> & transactions) {
    list<t_transaction>::iterator it = transactions.begin();

    while (it != transactions.end()) {
        if (!it->size()) {
            list<t_transaction>::iterator tmp = it++;
            transactions.erase(tmp);
        }
        else
            it++;
    }
}

void collect_trivial (list<t_transaction> & transactions,
                      map<t_component_id, t_knn> & knns) {
    list<t_transaction>::iterator it = transactions.begin();

    while (it != transactions.end()) {
        if (it->size() == 1) {
            t_observation & tmp = *it->begin();
            knns[tmp.first].insert(t_feedback_token(tmp.second, it->outcome));
            it->clear();
        }
        else
            it++;
    }
}

void collect_nontrivial (list<t_transaction> & transactions,
                         const map<t_component_id, t_knn> & knns,
                         map<t_component_id, t_knn> & knns_out) {
    list<t_transaction>::iterator it = transactions.begin();

    while (it != transactions.end()) {
        t_transaction::iterator tmp = it->begin();

        while (tmp != it->end()) {
            cout << tmp->first << endl;
            map<t_component_id, t_knn>::const_iterator knn = knns.find(tmp->first);

            if (knn == knns.end()) {
                tmp++;
                continue;
            }

            if (t_outcome outcome = (knn->second)(tmp->second, 3)) {
                knns_out[tmp->first].insert(t_feedback_token(tmp->second, outcome));
                t_transaction::iterator del_it = tmp++;
                it->erase(del_it);
            }
            else
                tmp++;
        }

        it++;
    }
}

void read_transaction (istream & in, t_transaction & transaction) {
    int outcome;


    in >> outcome;
    transaction.outcome = (t_outcome) outcome;

    while (in) {
        t_observation tmp;
        in >> tmp.first;

        if (!tmp.first)
            break;

        in >> tmp.second;
        transaction.push_back(tmp);
    }
}

int main () {
    list<t_transaction> transactions;
    map<t_component_id, t_knn> knns;
    map<t_component_id, t_knn> knns_out;

    while (cin) {
        transactions.push_back(t_transaction());

        t_transaction & tmp = *transactions.rbegin();
        read_transaction(cin, tmp);

        if (!tmp.size()) {
            transactions.pop_back();
            break;
        }
    }

    cout << transactions << std::endl;
    collect_trivial(transactions, knns);

    cout << transactions << std::endl;
    knns_out = knns;
    collect_nontrivial(transactions, knns, knns_out);

    cout << transactions << std::endl;

    cout << knns << std::endl;
    cout << knns_out << std::endl;

    return 0;
}