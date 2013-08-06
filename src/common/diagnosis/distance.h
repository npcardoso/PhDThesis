#ifndef __DIAGNOSIS_DISTANCE_H__
#define __DIAGNOSIS_DISTANCE_H__

#include "types.h"
#include "utils/boost.h"

namespace diagnosis {
typedef double t_data;
typedef boost::shared_ptr<const t_data[]> t_data_const_ptr;

typedef double t_distance;
typedef boost::shared_ptr<t_distance[]> t_distance_ptr;

class t_distance_function {
public:
    DEFINE_BOOST_SHARED_PTRS(t_distance_function);

    t_distance_function ();
    virtual ~t_distance_function ();

    virtual t_distance operator () (t_data_const_ptr data,
                                    t_id data_row,
                                    t_data_const_ptr centroids,
                                    t_id centroids_row,
                                    t_count dimensions) const = 0;
};

class t_euclidean_distance : public t_distance_function {
    virtual t_distance operator () (t_data_const_ptr data,
                                    t_id data_row,
                                    t_data_const_ptr centroids,
                                    t_id centroids_row,
                                    t_count dimensions) const;
};

class t_manhattan_distance : public t_distance_function {
    virtual t_distance operator () (t_data_const_ptr data,
                                    t_id data_row,
                                    t_data_const_ptr centroids,
                                    t_id centroids_row,
                                    t_count dimensions) const;
};
}

#endif