#ifndef __BENCHMARK_H__85256f48709bab034e918f5de145de7e4bc6af2b__
#define __BENCHMARK_H__85256f48709bab034e918f5de145de7e4bc6af2b__

#include "diagnosis/benchmark/benchmark_settings.h"
#include "diagnosis/randomizers/randomizer.h"


#include <boost/random/mersenne_twister.hpp>
namespace diagnosis {
namespace benchmark {
class t_benchmark {
public:

    const t_benchmark & operator () (randomizers::t_architecture & arch,
                                     boost::random::mt19937 & gen,
                                     t_benchmark_settings & settings) const;
};
}
}
#endif