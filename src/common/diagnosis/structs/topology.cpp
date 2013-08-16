#include "topology.h"

#include "utils/rgb.h"

#include <boost/foreach.hpp>
#include <boost/random/bernoulli_distribution.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
using namespace boost::random;

namespace diagnosis {
namespace structs {
t_fault::t_fault () {
    this->goodness = 0;
    this->failure = 1;
    this->e_min = 1;
    this->e_max = 1;
}

t_fault::t_fault (t_goodness goodness,
                  t_probability failure) {
    assert(goodness >= 0);
    assert(goodness <= 1);

    this->goodness = goodness;
    this->failure = failure;

    this->e_min = goodness ? 0 : 0.5;
    this->e_max = 1;
}

t_fault::t_fault (t_goodness goodness,
                  t_probability failure,
                  t_error e_min,
                  t_error e_max) {
    assert(!(e_min >= 0.5 && goodness > 0));
    assert(!(e_min < 0.5 && goodness == 0));
    assert(failure >= 0);
    assert(failure <= 1);
    assert(goodness >= 0);
    assert(goodness <= 1);
    assert(e_min >= 0);
    assert(e_max >= 0.5);
    assert(e_min <= e_max);
    assert(e_max <= 1);

    this->goodness = goodness;
    this->failure = failure;
    this->e_min = e_min;
    this->e_max = e_max;
}

t_error t_fault::gen_pass_value (boost::random::mt19937 & gen) const {
    assert(can_pass());

    if (e_min == 0.5)
        return 0.5;

    return uniform_real_distribution<t_error> (e_min, 0.5) (gen);
}

t_error t_fault::gen_error_value (boost::random::mt19937 & gen) const {
    if (e_max - e_min <= 0.00001)
        return e_max;

    return uniform_real_distribution<t_error> ((e_min > 0.5) ? e_min : 0.5, e_max) (gen);
}

bool t_fault::gen_failure (boost::random::mt19937 & gen) const {
    return bernoulli_distribution<> (failure) (gen);
}

bool t_fault::gen_error (boost::random::mt19937 & gen) const {
    return bernoulli_distribution<> (1 - goodness) (gen);
}

bool t_fault::can_pass () const {
    return e_min < 0.5 && goodness > 0;
}

t_link & t_link::add_sink (t_component_id comp, t_probability prob) {
    assert(prob >= 0);
    assert(prob <= 1);

    total += prob;
    sinks.push_back(t_sink(comp, prob));
    return *this;
}

t_component_id t_link::operator () (boost::random::mt19937 & gen) const {
    if (sinks.size() == 0)
        return 0;

    uniform_int_distribution<> dist(0, sinks.size() - 1);

    while (true) {
        t_component_id c = dist(gen);

        if (bernoulli_distribution<> (sinks[c].second / get_normalization_value()) (gen))
            return sinks[c].first;
    }

    return 0;
}

t_topology & t_topology::operator () (t_component_id comp, const t_fault & f) {
    assert(comp != 0);
    t_fault_bind::iterator it = fault_bind.find(comp);

    if (it != fault_bind.end())
        faults.erase(it->second);
    else
        components.insert(comp);


    fault_bind[comp] = faults.insert(faults.end(), f);
    return *this;
}

t_topology & t_topology::operator () (t_component_id comp, const t_link & l) {
    assert(comp != 0);
    t_interface_bind::iterator it = interface_bind.find(comp);

    if (it == interface_bind.end()) {
        interface_bind[comp] = interfaces.insert(interfaces.end(), t_interface());
        components.insert(comp);
    }

    BOOST_FOREACH(const t_link::t_sink & s, l.get_sinks()) {
        if (s.first)
            components.insert(s.first);
    }


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

const t_topology::t_components & t_topology::get_components () const {
    return components;
}

std::ostream & t_topology::graphviz (std::ostream & out) const {
    out << "digraph g {\n";
    BOOST_FOREACH(t_component_id c,
                  components) {
        graphviz_component(out, c);
    }

    BOOST_FOREACH(t_component_id c,
                  components) {
        graphviz_links(out, c);
    }

    out << "}\n";

    return out;
}

std::ostream & t_topology::graphviz_component (std::ostream & out, t_component_id comp) const {
    const t_fault * f = get_fault(comp);
    const t_interface * iface = get_interface(comp);


    out << "C" << comp << " [\n";
    out << "rankdir=LR,\n";
    out << "shape=record,\n";

    if (f)
        out << "color=\"#" << t_rgb(1 - f->get_goodness(), f->get_goodness(), 0) << "\",\n";
    else
        out << "color=\"#" << t_rgb(0, 1, 0) << "\",\n";

    out << "label=\"{<name> C" << comp;

    if (f) {
        out << "| {<goodness> " << f->get_goodness();
        out << "| <failure> " << f->get_failure();
        out << "} | {<emin> " << f->get_emin();
        out << "| <emax> " << f->get_emax() << "}";
    }

    if (iface) {
        t_id i = 1;
        BOOST_FOREACH(const t_link &l,
                      *iface) {
            out << " | <L" << i++ << "> ";
            BOOST_FOREACH(const t_link::t_sink & s, l.get_sinks()) {
                if (s.first)
                    out << " C" << s.first << ": " << s.second / l.get_normalization_value();
            }
        }
    }

    out << "}";

    out << "\"];\n";

    return out;
}

std::ostream & t_topology::graphviz_links (std::ostream & out, t_component_id comp) const {
    const t_interface * iface = get_interface(comp);


    if (iface) {
        t_id i = 1;
        BOOST_FOREACH(const t_link &l,
                      *iface) {
            BOOST_FOREACH(const t_link::t_sink & s, l.get_sinks()) {
                if (s.first) {
                    out << "C" << comp << ":L" << i << "->C" << s.first << ":name";
                    out << "[color=\" #" << t_rgb(0, 0, 0, s.second / l.get_normalization_value()) << "\"];\n";
                }
            }


            i++;
        }
    }

    return out;
}
}
}