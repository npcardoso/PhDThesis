#include "diagnostic_system.h"

#include "utils/iostream.h"

namespace diagnostic {

void t_diagnostic_system::add_generator (t_const_ptr<t_candidate_generator> generator) {
    generators.push_back(generator);
}

const t_diagnostic_system::t_generators & t_diagnostic_system::get_generators () const {
    return generators;
}

void t_diagnostic_system::add_ranker (t_const_ptr<t_candidate_ranker> ranker) {
    rankers.push_back(ranker);
}

const t_diagnostic_system::t_rankers & t_diagnostic_system::get_rankers () const {
    return rankers;
}

void t_diagnostic_system::add_connection (const t_connection & c) {
    assert(c.get_from() >= 0);
    assert(c.get_from() < generators.size());
    assert(c.get_to() >= 0);
    assert(c.get_to() < rankers.size());

    connections.push_back(c);
}

void t_diagnostic_system::add_connection (t_id generator_id, t_id ranker_id) {
    add_connection(t_connection(generator_id, ranker_id));

}

const t_diagnostic_system::t_connections & t_diagnostic_system::get_connections () const {
    return connections;
}
}

namespace std {

std::ostream& operator<<(std::ostream & s,
                         const diagnostic::t_candidate_generator& cg) {
    s << cg.to_string();
    return s;
}

std::ostream& operator<<(std::ostream & s,
                         const diagnostic::t_candidate_ranker & cr) {
    s << cr.to_string();
    return s;
}

std::ostream& operator<<(std::ostream& s,
                         const diagnostic::t_diagnostic_system & ds){
    s << "t_diagnostic_system[generators:" << ds.get_generators() << ", ";
    s << "rankers:" << ds.get_rankers() << ", ";
    s << "connections:" << ds.get_connections() << "]";
    return s;
}
}
