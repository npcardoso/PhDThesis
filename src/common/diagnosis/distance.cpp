#include "diagnosis/distance.h"

#include <cmath>

// row-major order
#define MATRIX_CELL(x, r, c, nc) x[r * nc + c]

namespace diagnosis {
t_distance_function::t_distance_function () {}

t_distance_function::~t_distance_function () {}

t_distance t_euclidean_distance::operator () (t_data_const_ptr data,
                                              t_id data_row,
                                              t_data_const_ptr centroids,
                                              t_id centroids_row,
                                              t_count dimensions) const {
    t_distance v, sum = 0;


    for (t_id i = 0; i < dimensions; i++) {
        v = MATRIX_CELL(data, data_row, i, dimensions) -
            MATRIX_CELL(centroids, centroids_row, i, dimensions);
        sum += v * v;
    }

    return sqrt(sum);
}

t_distance t_manhattan_distance::operator () (t_data_const_ptr data,
                                              t_id data_row,
                                              t_data_const_ptr centroids,
                                              t_id centroids_row,
                                              t_count dimensions) const {
    t_distance v, sum = 0;


    for (t_id i = 0; i < dimensions; i++) {
        v = MATRIX_CELL(data, data_row, i, dimensions) -
            MATRIX_CELL(centroids, centroids_row, i, dimensions);
        sum += std::fabs(v);
    }

    return sum;
}
}