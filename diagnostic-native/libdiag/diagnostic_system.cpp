#include "diagnostic_system.h"

#include "utils/iostream.h"

namespace diagnostic {

void t_diagnostic_system::add_generator (t_const_ptr<t_candidate_generator> generator) {
    return add_generator(generator,
                         boost::lexical_cast<std::string> (generators.size() + 1));
}

void t_diagnostic_system::add_generator (t_const_ptr<t_candidate_generator> generator,
                                         std::string name) {
    assert(get_generator_id(name) == 0);

    generators.push_back(generator);
    generator_names.push_back(name);
    generator_ids[name] = generators.size();
}

const t_diagnostic_system::t_generators & t_diagnostic_system::get_generators () const {
    return generators;
}

t_id t_diagnostic_system::get_generator_id (std::string name) const {
    t_id_map::const_iterator pos = generator_ids.find(name);


    if (pos == generator_ids.end())
        return 0;

    return pos->second;
}

std::string t_diagnostic_system::get_generator_name (t_id id) const {
    assert(id > 0);
    assert(id <= generator_names.size());

    return generator_names[id - 1];
}



void t_diagnostic_system::add_ranker (t_const_ptr<t_candidate_ranker> ranker) {
    return add_ranker(ranker,
                      boost::lexical_cast<std::string> (rankers.size() + 1));
}

void t_diagnostic_system::add_ranker (t_const_ptr<t_candidate_ranker> ranker,
                                      std::string name) {
    assert(get_ranker_id(name) == 0);

    rankers.push_back(ranker);
    ranker_names.push_back(name);
    ranker_ids[name] = rankers.size();
}

const t_diagnostic_system::t_rankers & t_diagnostic_system::get_rankers () const {
    return rankers;
}


t_id t_diagnostic_system::get_ranker_id (std::string name) const {
    t_id_map::const_iterator pos = ranker_ids.find(name);


    if (pos == ranker_ids.end())
        return 0;

    return pos->second;
}

std::string t_diagnostic_system::get_ranker_name (t_id id) const {
    assert(id > 0);
    assert(id <= ranker_names.size());

    return ranker_names[id - 1];
}


void t_diagnostic_system::add_connection (const t_connection & c) {
    assert(c.get_from() > 0);
    assert(c.get_from() <= generators.size());
    assert(c.get_to() > 0);
    assert(c.get_to() <= rankers.size());

    connections.push_back(c);
}

void t_diagnostic_system::add_connection (t_id generator_id, t_id ranker_id) {
    add_connection(t_connection(generator_id, ranker_id));

}

void t_diagnostic_system::add_connection (std::string generator_id,
                                          std::string ranker_id) {
    add_connection(get_generator_id(generator_id),
                   get_ranker_id(ranker_id));
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
