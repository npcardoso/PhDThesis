#include "topology_based.h"

#include <boost/random/bernoulli_distribution.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <stack>

using namespace boost::random;
using namespace diagnosis::structs;

namespace diagnosis {
namespace randomizers {
t_topology_based::t_topology_based (structs::t_topology::t_ptr & topology) : topology(topology) {
    stack_size = 0;
    max_transactions = 0;
    until_nerrors = 0;
}

t_component_id t_topology_based::gen_entry_point (boost::random::mt19937 & gen) const {
    assert(entry_points.size());
    uniform_int_distribution<> comp_selector(0, entry_points.size() - 1);

    while (true) {
        t_component_id selected = comp_selector(gen);

        if (bernoulli_distribution<> (entry_points[selected].second) (gen))
            return entry_points[selected].first;
    }

    return 0;
}

t_topology_based & t_topology_based::add_entry_point (t_component_id comp,
                                                      t_probability prob) {
    entry_points.push_back(t_entry_point(comp, prob));
    return *this;
}

t_topology_based & t_topology_based::set_stack_size (t_count size) {
    stack_size = size;
    return (*this);
}

t_topology_based & t_topology_based::set_max_transactions (t_count max_transactions) {
    this->max_transactions = max_transactions;
    return (*this);
}

t_topology_based & t_topology_based::set_until_nerrors (t_count nerrors) {
    until_nerrors = nerrors;
    return (*this);
}

const t_topology_based::t_self_type & t_topology_based::operator () (structs::t_count_spectra & spectra,
                                                                     t_candidate & correct_candidate,
                                                                     boost::random::mt19937 & gen,
                                                                     t_transaction_id tran) const {
    typedef std::pair<t_topology::t_interface::const_iterator, t_topology::t_interface::const_iterator> t_stack_element;

    std::stack<t_stack_element> stack;

    t_component_id comp = gen_entry_point(gen);

    bool fail = false;
    spectra.set_error(tran, 0);
    spectra.set_confidence(tran, 1);

    do {
        const t_topology::t_interface * iface = topology->get_interface(comp);
        const t_fault * fault = topology->get_fault(comp);

        spectra.hit(comp, tran);

        if (fault) {
            bool err = fault->gen_error(gen);
            t_error tmp_err_value = spectra.get_error(tran);

            if (err) {
                fail = fault->gen_failure(gen);
                correct_candidate.insert(comp);

                if (fail)
                    tmp_err_value = 1;
                else
                    tmp_err_value += fault->gen_error_value(gen);
            }
            else {
                tmp_err_value += fault->gen_pass_value(gen);
            }

            spectra.set_error(tran, tmp_err_value > 1 ? 1 : tmp_err_value);
        }

        if (iface)
            stack.push(t_stack_element(iface->begin(),
                                       iface->end()));

        comp = 0;

        // Find next component to analyze
        while (comp == 0 && stack.size()) {
            if (stack.top().first == stack.top().second)
                stack.pop();
            else {
                comp = (*stack.top().first)(gen);
                stack.top().first++;
            }
        }
    } while (!fail && stack.size());

    return *this;
}

const t_topology_based::t_self_type & t_topology_based::operator () (structs::t_count_spectra & spectra,
                                                                     t_candidate & correct_candidate,
                                                                     boost::random::mt19937 & gen) const {
    assert(entry_points.size());
    assert(until_nerrors || max_transactions);

    spectra.set_element_count(*topology->get_components().rbegin(), 0);

    while (true) {
        spectra.new_transaction();
        (* this)(spectra, correct_candidate, gen, spectra.get_transaction_count());

        if (until_nerrors && spectra.get_error_count() == until_nerrors)
            break;

        if (max_transactions && spectra.get_transaction_count() == max_transactions)
            break;
    }

    return *this;
}
}
}