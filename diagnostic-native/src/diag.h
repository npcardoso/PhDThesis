#ifndef __DIAG_H_ba433484e8f1aaee7da1aeb7c87e0dd20ec02b19__
#define __DIAG_H_ba433484e8f1aaee7da1aeb7c87e0dd20ec02b19__

#include "types.h"

// Benchmark Stuff
#include "benchmark/path_generator.h"
#include "benchmark/generators/generator.h"
#include "benchmark/generators/replay.h"
#include "benchmark/benchmark.h"
#include "benchmark/benchmark_settings.h"

// Benchmark Hooks
#include "benchmark/hooks/job_tracker.h"
#include "benchmark/hooks/flusher.h"
#include "benchmark/hooks/hook_combiner.h"
#include "benchmark/hooks/metrics_hook.h"
#include "benchmark/hooks/verbose_hook.h"
#include "benchmark/hooks/statistics_hook.h"
#include "benchmark/hooks/save_hook.h"
#include "benchmark/hooks/statistics_hook.h"
#include "benchmark/metrics.h"

// Spectrum Generators
#include "benchmark/generators/bernoulli.h"
#include "benchmark/generators/fuzzy_bernoulli.h"
#include "benchmark/generators/replay.h"
#include "benchmark/generators/topology_based.h"

// Algorithms
#include "algorithms/single_fault.h"
#include "algorithms/mhs.h"
#include "algorithms/similarity.h"
#include "algorithms/barinel.h"

// Structs
#include "structs/count_spectrum.h"

// Configuration
#include "configuration/configuration.h"



#endif
