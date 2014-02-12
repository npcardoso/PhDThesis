#ifndef __BENCHMARK_H__85256f48709bab034e918f5de145de7e4bc6af2b__
#define __BENCHMARK_H__85256f48709bab034e918f5de145de7e4bc6af2b__

#include "diagnosis/benchmark/benchmark_settings.h"
#include "diagnosis/randomizers/randomizer.h"
#include "utils/multithread.h"


#include <boost/random/mersenne_twister.hpp>
namespace diagnosis {
namespace benchmark {
class t_generator_job : public t_job {
public:
    t_generator_job (t_id generator_id,
                     const t_benchmark_settings & settings,
                     const t_status_iteration_init::t_const_ptr & status);

    void operator () (t_execution_controller & controller) const;
    std::string get_type () const;
    virtual bool operator < (const t_job & job) const;

private:
    t_id generator_id;
    const t_benchmark_settings & settings;
    const t_status_iteration_init::t_const_ptr status;
};


class t_ranker_job : public t_job {
public:
    t_ranker_job (t_id ranker_id,
                  const t_benchmark_settings & settings,
                  const t_status_post_gen::t_const_ptr & status);

    void operator () (t_execution_controller & controller) const;
    std::string get_type () const;
    virtual bool operator < (const t_job & job) const;

private:
    t_id ranker_id;
    const t_benchmark_settings & settings;
    const t_status_post_gen::t_const_ptr status;
};

class t_benchmark {
public:

    const t_benchmark & operator () (randomizers::t_architecture & arch,
                                     boost::random::mt19937 & gen,
                                     t_benchmark_settings & settings) const;
};
}
}
#endif