#include "topology.h"

#include "utils/rgb.h"

#include <boost/foreach.hpp>
#include <boost/random/bernoulli_distribution.hpp>
#include <boost/random/discrete_distribution.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>

using namespace boost::random;

namespace diagnosis {
namespace structs {
t_fault::t_fault () {
    this->pass_prob = 0.5;
    this->soft_prob = 0;
    this->hard_prob = 0.5;
    this->fail_prob = 0;
}

t_fault::t_fault (t_goodness goodness,
                  t_probability fail_prob) {
    assert(goodness >= 0);
    assert(goodness <= 1);

    this->pass_prob = goodness;
    this->soft_prob = 0;
    this->hard_prob = 1 - goodness;
    this->fail_prob = fail_prob;
}

t_fault::t_fault (t_probability pass_prob,
                  t_probability soft_prob,
                  t_probability hard_prob,
                  t_probability fail_prob) {
    assert(pass_prob >= 0);
    assert(pass_prob <= 1);
    assert(soft_prob >= 0);
    assert(soft_prob <= 1);
    assert(hard_prob >= 0);
    assert(hard_prob <= 1);
    assert(fail_prob >= 0);
    assert(fail_prob <= 1);

    t_probability total = pass_prob + soft_prob + hard_prob;

    assert(total > 0);

    this->pass_prob = pass_prob / total;
    this->soft_prob = soft_prob / total;
    this->hard_prob = hard_prob / total;
    this->fail_prob = fail_prob;
}

t_error t_fault::gen_error (boost::random::mt19937 & gen) const {
    double probabilities[] = {pass_prob, soft_prob, hard_prob};
    boost::random::discrete_distribution<> dist(probabilities);


    switch (dist(gen)) {
    case 0:
        return 0;

    case 1:
        return uniform_real_distribution<t_error> (0, 1) (gen);

    case 2:
        return 1;
    }

    assert(false);
    return 0;
}

bool t_fault::gen_failure (boost::random::mt19937 & gen) const {
    return bernoulli_distribution<> (fail_prob) (gen);
}

t_link & t_link::add_sink (t_component_id comp, t_probability prob) {
    assert(prob >= 0);
    assert(prob <= 1);

    total += prob;
    sinks.push_back(t_sink(comp, prob));
    std::sort(sinks.begin(), sinks.end()); // TODO: Can be optimized
    return *this;
}

t_probability t_link::get_probability (t_component_id comp) const {
    t_sinks::const_iterator it = std::lower_bound(sinks.begin(), sinks.end(), t_sink(comp, 0));


    if (it == sinks.end()) // Not found
        return 0;

    return it->second / get_normalization_value();
}

t_component_id t_link::gen_destination (boost::random::mt19937 & gen) const {
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

t_topology & t_topology::add_fault (t_component_id comp, const t_fault & f) {
    assert(comp != 0);
    t_fault_bind::iterator it = fault_bind.find(comp);

    if (it != fault_bind.end())
        faults.erase(it->second);
    else
        components.insert(comp);


    fault_bind[comp] = faults.insert(faults.end(), f);
    return *this;
}

t_topology & t_topology::add_link (t_component_id comp, const t_link & l) {
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

t_topology & t_topology::add_entry_point (t_component_id comp, t_probability prob) {
    assert(comp != 0);
    components.insert(comp);
    entry_points.add_sink(comp, prob);
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
        out << "color=\"#" << t_rgb(f->get_hard_prob(), 1 - f->get_hard_prob(), 0) << "\",\n";
    else
        out << "color=\"#" << t_rgb(0, 1, 0) << "\",\n";

    out << "label=\"{<name> C" << comp;
    t_probability prob = entry_points.get_probability(comp);

    if (prob > 0)
        out << " - Entry: " << prob << " ";

    if (f) {
        out << "| {<pass> p:" << f->get_pass_prob();
        out << "| <soft> s:" << f->get_soft_prob();
        out << "| <hard> h:" << f->get_hard_prob();
        out << "| <fail> f:" << f->get_fail_prob() << "}";
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

t_component_id t_topology::gen_entry_point (boost::random::mt19937 & gen) const {
    return entry_points.gen_destination(gen);
}
}
}