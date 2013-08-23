#ifndef __TOPOLOGIES_H_5297dae76cee7fb6b71d29e25f33dca1337a8ffd__
#define __TOPOLOGIES_H_5297dae76cee7fb6b71d29e25f33dca1337a8ffd__

#include <boost/random/mersenne_twister.hpp>
#include "diagnosis/structs/topology.h"

using namespace boost::random;
using namespace diagnosis::structs;

void forwarding_network_topology (mt19937 & gen,
                                  t_topology::t_ptr & topology,
                                  const t_fault & fault,
                                  t_count levels,
                                  t_count per_level,
                                  t_count n_faults);

#endif