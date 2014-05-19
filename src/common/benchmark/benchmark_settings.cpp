#include "benchmark/benchmark_settings.h"

namespace diagnosis {
namespace benchmark {
t_benchmark_settings::t_benchmark_settings (const t_ptr<t_collector> & collector,
                                            const t_const_ptr<t_benchmark_hook> & hook,
                                            const t_ptr<t_job_queue> & job_queue) : collector(collector), hook(hook), job_queue(job_queue) {}


void t_benchmark_settings::add_generator (const t_const_ptr<t_candidate_generator> & generator) {
    return add_generator(generator,
                         boost::lexical_cast<std::string> (generators.size() + 1));
}

void t_benchmark_settings::add_generator (const t_const_ptr<t_candidate_generator> & generator,
                                          const std::string & name) {
    assert(get_generator_id(name) == 0);

    generators.push_back(generator);
    generator_names.push_back(name);
    generator_ids[name] = generators.size();

    connections.push_back(t_ranker_list());

    assert(generators.size() == connections.size());
}

void t_benchmark_settings::add_ranker (const t_const_ptr<t_candidate_ranker> & ranker) {
    return add_ranker(ranker,
                      boost::lexical_cast<std::string> (rankers.size() + 1));
}

void t_benchmark_settings::add_ranker (const t_const_ptr<t_candidate_ranker> & ranker,
                                       const std::string & name) {
    assert(get_ranker_id(name) == 0);

    rankers.push_back(ranker);
    ranker_names.push_back(name);
    ranker_ids[name] = rankers.size();
}

const t_const_ptr<t_candidate_generator> & t_benchmark_settings::get_generator (t_id generator_id) const {
    assert(generator_id > 0);
    assert(generator_id <= generators.size());
    return generators[generator_id - 1];
}

t_id t_benchmark_settings::get_generator_id (const std::string & name) const {
    t_id_map::const_iterator pos = generator_ids.find(name);


    if (pos == generator_ids.end())
        return 0;

    return pos->second;
}

const std::string & t_benchmark_settings::get_generator_name (t_id id) const {
    assert(id > 0);
    assert(id <= generator_names.size());

    return generator_names[id - 1];
}

t_count t_benchmark_settings::get_generator_count () const {
    return generators.size();
}

const t_benchmark_settings::t_ranker_list & t_benchmark_settings::get_connections (t_id generator_id) const {
    assert(generator_id > 0);
    assert(generator_id <= connections.size());

    return connections[generator_id - 1];
}

const t_const_ptr<t_candidate_ranker> & t_benchmark_settings::get_ranker (t_id ranker_id) const {
    assert(ranker_id > 0);
    assert(ranker_id <= rankers.size());

    return rankers[ranker_id - 1];
}

t_id t_benchmark_settings::get_ranker_id (const std::string & name) const {
    t_id_map::const_iterator pos = ranker_ids.find(name);


    if (pos == ranker_ids.end())
        return 0;

    return pos->second;
}

const std::string & t_benchmark_settings::get_ranker_name (t_id id) const {
    assert(id > 0);
    assert(id <= ranker_names.size());

    return ranker_names[id - 1];
}

void t_benchmark_settings::add_connection (t_id generator_id, t_id ranker_id) {
    assert(generator_id > 0);
    assert(generator_id <= generators.size());
    assert(generator_id <= connections.size());

    assert(ranker_id > 0);
    assert(ranker_id <= rankers.size());

    connections[generator_id - 1].push_back(ranker_id);
}

void t_benchmark_settings::add_connection (const std::string & generator_id,
                                           const std::string & ranker_id) {
    return add_connection(get_generator_id(generator_id),
                          get_ranker_id(ranker_id));
}

t_collector & t_benchmark_settings::get_collector () const {
    return *collector;
}

const t_benchmark_hook & t_benchmark_settings::get_hook () const {
    return *hook;
}

t_job_queue & t_benchmark_settings::get_job_queue () const {
    return *job_queue;
}
}
}