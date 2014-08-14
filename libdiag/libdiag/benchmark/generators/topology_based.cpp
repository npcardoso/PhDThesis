#include "topology_based.h"


#include <stack>

using namespace boost::random;
using namespace diagnostic::structs;

namespace diagnostic {
namespace benchmark {
t_topology_based_generator::t_topology_based_generator () {
    stack_size = 0;
    max_transactions = 0;
    until_nerrors = 0;
}

t_topology_based_generator::t_topology_based_generator (const t_const_ptr<structs::t_topology> & topology) : topology(topology) {
    stack_size = 0;
    max_transactions = 0;
    until_nerrors = 0;
}

void t_topology_based_generator::set_topology (const t_const_ptr<structs::t_topology> & topology) {
    this->topology = topology;
}

void t_topology_based_generator::set_topology (const structs::t_topology * topology) {
    this->topology = t_const_ptr<structs::t_topology> (topology);
}

void t_topology_based_generator::set_stack_size (t_count size) {
    stack_size = size;
}

void t_topology_based_generator::set_max_transactions (t_count max_transactions) {
    this->max_transactions = max_transactions;
}

void t_topology_based_generator::set_max_activations (t_count max_activations) {
    this->max_activations = max_activations;
}

void t_topology_based_generator::set_until_nerrors (t_count nerrors) {
    until_nerrors = nerrors;
}

void t_topology_based_generator::operator () (structs::t_count_spectra & spectra,
                                              t_candidate & correct_candidate,
                                              std::mt19937 & gen,
                                              t_transaction_id tran) const {
    assert(topology);
    typedef std::pair<t_topology::t_interface::const_iterator, t_topology::t_interface::const_iterator> t_stack_element;

    std::stack<t_stack_element> stack;

    t_count activations = 0;
    t_component_id comp = topology->gen_entry_point(gen);
    assert(comp);

    bool fail = false;
    spectra.set_error(tran, 0);
    spectra.set_confidence(tran, 1);

    do {
        const t_topology::t_interface * iface = topology->get_interface(comp);
        const t_fault * fault = topology->get_fault(comp);

        spectra.hit(comp, tran);
        activations++;

        if (fault) {
            t_error e = fault->gen_error(gen);
            spectra.set_error(tran, std::max(e, spectra.get_error(tran)));

            if (e == 1)
                correct_candidate.insert(comp);
        }

        // Max activations check
        if (max_activations && activations > max_activations)
            break;

        if (iface)
            stack.push(t_stack_element(iface->begin(),
                                       iface->end()));

        comp = 0;

        // Find next component to analyze
        while (comp == 0 && stack.size()) {
            if (stack.top().first == stack.top().second)
                stack.pop();
            else {
                comp = stack.top().first->gen_destination(gen);
                stack.top().first++;
            }
        }
    } while (!fail && stack.size());
}

structs::t_spectra * t_topology_based_generator::operator () (std::mt19937 & gen,
                                                              structs::t_candidate & correct_candidate) {
    assert(topology);
    assert(until_nerrors || max_transactions);

    t_count_spectra & spectra = *(new t_count_spectra(*topology->get_components().rbegin(), 0));

    while (true) {
        spectra.new_transaction();
        (* this)(spectra, correct_candidate, gen, spectra.get_transaction_count());

        if (until_nerrors && spectra.get_error_count() == until_nerrors)
            break;

        if (max_transactions && spectra.get_transaction_count() == max_transactions)
            break;
    }

    return &spectra;
}

std::ostream & t_topology_based_generator::write (std::ostream & out) const {
    return topology->graphviz(out); // TODO: replace with serialization format
}
}
}
