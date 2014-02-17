#ifndef __BENCHMARK_H__85256f48709bab034e918f5de145de7e4bc6af2b__
#define __BENCHMARK_H__85256f48709bab034e918f5de145de7e4bc6af2b__

#include "diagnosis/benchmark/benchmark_settings.h"
#include "diagnosis/benchmark/generators/generator.h"

namespace diagnosis {
namespace benchmark {
// TODO: change name
void run_benchmark (t_spectra_generator & generator,
                    std::mt19937 & gen,
                    t_benchmark_settings & settings,
                    t_execution_controller & controller);
}
}
#endif