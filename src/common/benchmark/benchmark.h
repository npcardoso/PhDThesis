#ifndef __BENCHMARK_H__85256f48709bab034e918f5de145de7e4bc6af2b__
#define __BENCHMARK_H__85256f48709bab034e918f5de145de7e4bc6af2b__

#include "benchmark_settings.h"
#include "generators/generator.h"

namespace diagnosis {
namespace benchmark {
void run_benchmark (const t_benchmark_settings & settings,
                    t_spectra_generator & generator,
                    std::mt19937 & gen,
                    t_execution_controller & controller);


inline void run_benchmark (const t_benchmark_settings & settings,
                           t_spectra_generator & generator,
                           std::mt19937 & gen) {
    t_count processors = std::thread::hardware_concurrency();


    if (!processors)
        processors = 1;

    t_execution_controller * controller = new t_execution_controller(processors);


    run_benchmark(settings, generator, gen, *controller);
    delete controller;
}

inline void run_benchmark (const t_benchmark_settings & settings,
                           t_spectra_generator & generator,
                           std::mt19937 & gen,
                           t_count processors) {
    t_execution_controller * controller = new t_execution_controller(processors);


    run_benchmark(settings, generator, gen, *controller);
    delete controller;
}
}
}
#endif