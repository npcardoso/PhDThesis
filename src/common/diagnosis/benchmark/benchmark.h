#ifndef __BENCHMARK_H__85256f48709bab034e918f5de145de7e4bc6af2b__
#define __BENCHMARK_H__85256f48709bab034e918f5de145de7e4bc6af2b__

#include "diagnosis/benchmark/benchmark_hook.h"
#include "diagnosis/randomizers/randomizer.h"


#include <vector>
#include <set>
#include <boost/random/mersenne_twister.hpp>

namespace diagnosis {
namespace benchmark {
class t_benchmark {
public:
    typedef std::pair<t_id, t_id> t_connection;

    const t_benchmark & operator () (randomizers::t_randomizer<randomizers::t_spectra_randomizer> & meta_randomizer,
                                     boost::random::mt19937 & gen,
                                     t_benchmark_hook & hook,
                                     t_count systems,
                                     t_count iterations) const;

    const t_benchmark & operator () (const randomizers::t_spectra_randomizer & randomizer,
                                     boost::random::mt19937 & gen,
                                     t_benchmark_hook & hook,
                                     t_count iterations) const;

    const t_benchmark & operator () (const structs::t_spectra & spectra,
                                     const structs::t_candidate & correct,
                                     t_benchmark_hook & hook) const;
    t_benchmark & add_generator (t_candidate_generator::t_ptr & generator);
    t_benchmark & add_generator (t_candidate_generator::t_ptr & generator,
                                 const std::string & name);

    t_benchmark & add_ranker (t_candidate_ranker::t_ptr & ranker);
    t_benchmark & add_ranker (t_candidate_ranker::t_ptr & ranker,
                              const std::string & name);

    t_id get_generator_id (const std::string & name) const;
    t_id get_ranker_id (const std::string & name) const;

    t_benchmark & add_connection (t_id generator_id,
                                  t_id ranker_id);
    t_benchmark & add_connection (const std::string & generator_id,
                                  const std::string & ranker_id);


protected:
    std::set<t_connection> connections;

    typedef std::map<std::string, t_id> t_id_map;
    t_id_map generator_ids;
    t_id_map ranker_ids;

    std::vector<t_candidate_generator::t_ptr> generators;
    std::vector<t_candidate_ranker::t_ptr> rankers;
};
}
}
#endif