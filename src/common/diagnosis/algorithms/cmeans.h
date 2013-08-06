#ifndef __DIAGNOSIS_ALGORITHMS_CMEANS_H__
#define __DIAGNOSIS_ALGORITHMS_CMEANS_H__

#include "types.h"
#include "utils/boost.h"
#include "diagnosis/distance.h"
#include "diagnosis/spectra/count_spectra.h"
#include "diagnosis/structs/membership.h"

namespace diagnosis {
namespace algorithms {
typedef double t_data;
typedef boost::shared_ptr<t_data[]> t_data_ptr;
typedef boost::shared_ptr<const t_data[]> t_data_const_ptr;

class t_cmeans_configs {
public:
    t_cmeans_configs ();

    t_count num_iterations, locked_centroids;
    t_data m, epsilon;
    t_distance_function * dist_function;
};

class t_cmeans {
public:
    t_cmeans (t_count points, t_count dimensions, t_data_const_ptr data);

    t_cmeans (t_count_spectra & spectra);

    structs::t_membership clustering (t_count num_centroids,
                                      t_data_ptr centroids,
                                      t_cmeans_configs configs=t_cmeans_configs());

    structs::t_membership clustering (t_count num_centroids,
                                      t_cmeans_configs configs=t_cmeans_configs());

    structs::t_membership clustering (t_cmeans_configs configs=t_cmeans_configs());

    t_data_ptr get_spectra_centroids (const t_count_spectra & spectra) const;

private:
    t_data_ptr initial_centroids (t_id num_centroids) const;

    void update_centroids ();

    void calculate_memberships ();

    t_data calculate_error () const;

    t_data_const_ptr data;
    t_data_ptr centroids;
    t_distance_ptr distances;
    structs::t_membership memberships;
    t_count num_centroids, points, dimensions;
    t_cmeans_configs configs;
};
}
}

#endif