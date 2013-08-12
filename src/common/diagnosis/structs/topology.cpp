#include "topology.h"
#include <boost/random/bernoulli_distribution.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
using namespace boost::random;

namespace diagnosis {
namespace structs {
t_fault::t_fault () {
    this->goodness = 0;
    this->e_min = 1;
    this->e_max = 1;
}

t_fault::t_fault (t_goodness goodness) {
    assert(goodness >= 0);
    assert(goodness <= 1);

    this->goodness = goodness;

    if (goodness) {
        this->e_min = 0;
        this->e_max = 1;
    }
}

t_fault::t_fault (t_goodness goodness,
                  t_error e_min,
                  t_error e_max) {
    this->goodness = goodness;
    this->e_min = e_min;
    this->e_max = e_max;

    assert(!(e_min >= 0.5 && goodness > 0));
    assert(!(e_min < 0.5 && goodness == 0));
    assert(goodness >= 0);
    assert(goodness <= 1);
    assert(e_min >= 0);
    assert(e_max >= 0.5);
    assert(e_min <= e_max);
    assert(e_max <= 1);
}

t_error t_fault::gen_pass_value (boost::random::mt19937 & gen) const {
    assert(can_pass());

    if (e_min == 0.5)
        return 0.5;

    uniform_real_distribution<t_error> error(e_min, 0.5);

    return error(gen);
}

t_error t_fault::gen_error_value (boost::random::mt19937 & gen) const {
    if (e_max - e_min <= 0.00001)
        return e_max;

    uniform_real_distribution<t_error> error((e_min > 0.5) ? e_min : 0.5, e_max);

    return error(gen);
}

bool t_fault::gen_error (boost::random::mt19937 & gen) const {
    bernoulli_distribution<> error(1 - goodness);

    return error(gen);
}

t_goodness t_fault::get_goodness () const {
    return goodness;
}

bool t_fault::can_pass () const {
    return e_min < 0.5 && goodness > 0;
}

t_component_id t_link::operator () (boost::random::mt19937 & gen) {
    return bernoulli_distribution<> (prob)(gen) ? branch1 : branch2;
}

t_topology & t_topology::operator () (t_component_id comp, const t_fault & f) {
    t_fault_bind::iterator it = fault_bind.find(comp);


    if (it != fault_bind.end())
        faults.erase(it->second);


    fault_bind[comp] = faults.insert(faults.end(), f);
    return *this;
}

t_topology & t_topology::operator () (t_component_id comp, const t_link & l) {
    t_interface_bind::iterator it = interface_bind.find(comp);


    if (it == interface_bind.end())
        interface_bind[comp] = interfaces.insert(interfaces.end(), t_interface());


    interface_bind[comp]->push_back(l);

    return *this;
}

const t_topology::t_interface * t_topology::get_interface (t_component_id comp) const {
    t_interface_bind::const_iterator it = interface_bind.find(comp);


    if (it == interface_bind.end())
        return NULL;
    else return &(*it->second);
}

const t_fault * t_topology::get_fault (t_component_id comp) const {
    t_fault_bind::const_iterator it = fault_bind.find(comp);


    if (it == fault_bind.end())
        return NULL;
    else return &(*it->second);
}
}
}