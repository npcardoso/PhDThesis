#ifndef __BENCHMARK_SETTINGS_H_6dc7d2b886ba77e1f76d2b0061b0ffaecab2961b__
#define __BENCHMARK_SETTINGS_H_6dc7d2b886ba77e1f76d2b0061b0ffaecab2961b__

#include "diagnosis/benchmark/hooks/benchmark_hook.h"
#include "diagnosis/benchmark/collector.h"

#include <vector>
#include <map>
#include <list>


namespace diagnosis {
namespace benchmark {
class t_benchmark_settings {
public:
    typedef std::list<t_id> t_ranker_list;

    t_benchmark_settings (const t_collector::t_ptr & collector,
                          const t_benchmark_hook::t_const_ptr & hook);

    void add_generator (t_candidate_generator::t_const_ptr & generator);
    void add_generator (t_candidate_generator::t_const_ptr & generator,
                        const std::string & name);

    void add_ranker (t_candidate_ranker::t_const_ptr & ranker);
    void add_ranker (t_candidate_ranker::t_const_ptr & ranker,
                     const std::string & name);

    const t_candidate_generator::t_const_ptr & get_generator (t_id generator_id) const;
    t_id get_generator_id (const std::string & name) const;
    const std::string & get_generator_name (t_id generator_id) const;
    t_count get_generator_count () const;
    const t_ranker_list & get_connections (t_id generator_id) const;

    const t_candidate_ranker::t_const_ptr & get_ranker (t_id ranker_id) const;
    t_id get_ranker_id (const std::string & name) const;
    const std::string & get_ranker_name (t_id ranker_id) const;

    void add_connection (t_id generator_id,
                         t_id ranker_id);
    void add_connection (const std::string & generator_id,
                         const std::string & ranker_id);


    t_collector & get_collector () const;
    const t_benchmark_hook & get_hook () const;


protected:
    typedef std::vector<t_ranker_list> t_connections;
    t_connections connections;

    typedef std::map<std::string, t_id> t_id_map;
    t_id_map generator_ids;
    t_id_map ranker_ids;

    typedef std::vector<std::string> t_name_vector;
    t_name_vector generator_names;
    t_name_vector ranker_names;

    std::vector<t_candidate_generator::t_const_ptr> generators;
    std::vector<t_candidate_ranker::t_const_ptr> rankers;

    t_collector::t_ptr collector;
    t_benchmark_hook::t_const_ptr hook;
};
}
}
#endif