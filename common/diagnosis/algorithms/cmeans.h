#ifndef __DIAGNOSIS_ALGORITHMS_CMEANS_H__
#define __DIAGNOSIS_ALGORITHMS_CMEANS_H__

#include "utils/boost.h"

typedef double t_value;
typedef unsigned int t_index;

//row-major order
#define MATRIX_CELL(x, r, c, nc) x[r * nc + c]

typedef boost::shared_ptr<t_value[]> t_value_ptr;
typedef boost::shared_ptr<const t_value[]> t_value_const_ptr;

typedef t_value (*f_distance)(t_value_const_ptr data, 
                              t_index data_row,
                              t_value_const_ptr centroids,
                              t_index centroids_row,
                              t_index dimensions);

namespace diagnosis {
namespace algorithms {

  t_value euclidean_distance(t_value_const_ptr data, 
                                    t_index data_row,
                                    t_value_const_ptr centroids,
                                    t_index centroids_row,
                                    t_index dimensions);

  t_value manhattan_distance(t_value_const_ptr data, 
                                    t_index data_row,
                                    t_value_const_ptr centroids,
                                    t_index centroids_row,
                                    t_index dimensions);

  class t_cmeans_options {
  public:
    t_cmeans_options() {
      initialize_defaults();
    }

    t_index num_iterations, locked_centroids;
    t_value m, epsilon;
    f_distance dist_function;
    
  private:
    inline void initialize_defaults() {
      num_iterations = 50;
      dist_function = euclidean_distance;
      m = 2;
      epsilon = 0.0001;
      locked_centroids = 0;
    }
  };
  
  class t_cmeans {
  public:
    t_cmeans(t_index points, t_index dimensions, t_value_const_ptr data) : 
      points(points),
      dimensions(dimensions),
      data(data) { }
    
    t_value_ptr clustering(t_index num_centroids, 
                        t_value_ptr centroids,
                        t_cmeans_options options = t_cmeans_options());
                        
    inline t_value_ptr clustering(t_index num_centroids,
                               t_cmeans_options options = t_cmeans_options()) {
      t_value_ptr centroids = initial_centroids(num_centroids);
      return clustering(num_centroids, centroids, options);
    }

  private:
    t_value_ptr initial_centroids(t_index num_centroids);
    
    void update_centroids();
                                  
    void calculate_memberships();
                               
    inline t_value calculate_error() {
      t_value sum = 0;
      
      for(t_index i = 0; i < points; i++)
        for(t_index j = 0; j < num_centroids; j++)
          sum += MATRIX_CELL(memberships,i,j,num_centroids) * 
                 MATRIX_CELL(distances,i,j,num_centroids);
      
      return sum;
    }

    t_value_const_ptr data;
    t_value_ptr centroids, distances, memberships;
    t_index num_centroids, points, dimensions;
    t_cmeans_options options;
  };

}
}

#endif