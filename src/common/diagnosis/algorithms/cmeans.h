#ifndef __DIAGNOSIS_ALGORITHMS_CMEANS_H__
#define __DIAGNOSIS_ALGORITHMS_CMEANS_H__

#include "types.h"
#include "utils/boost.h"

typedef double t_value;

//row-major order
#define MATRIX_CELL(x, r, c, nc) x[r * nc + c]

namespace diagnosis {
namespace algorithms {
  
  typedef double t_distance;
  typedef boost::shared_ptr<t_distance[]> t_distance_ptr;
  
  typedef double t_membership;
  typedef boost::shared_ptr<t_membership[]> t_membership_ptr;
  
  typedef boost::shared_ptr<t_value[]> t_value_ptr;
  typedef boost::shared_ptr<const t_value[]> t_value_const_ptr;

  typedef t_distance (*f_distance)(t_value_const_ptr data, 
                                   t_id data_row,
                                   t_value_const_ptr centroids,
                                   t_id centroids_row,
                                   t_count dimensions);

  t_distance euclidean_distance(t_value_const_ptr data, 
                                t_id data_row,
                                t_value_const_ptr centroids,
                                t_id centroids_row,
                                t_count dimensions);

  t_distance manhattan_distance(t_value_const_ptr data, 
                                t_id data_row,
                                t_value_const_ptr centroids,
                                t_id centroids_row,
                                t_count dimensions);

  class t_cmeans_options {
  public:
    t_cmeans_options() {
      initialize_defaults();
    }

    t_count num_iterations, locked_centroids;
    t_value m, epsilon;
    f_distance dist_function;
    
  private:
    void initialize_defaults() {
      num_iterations = 50;
      dist_function = euclidean_distance;
      m = 2;
      epsilon = 0.0001;
      locked_centroids = 0;
    }
  };
  
  class t_cmeans {
  public:
    t_cmeans(t_count points, t_count dimensions, t_value_const_ptr data) : 
      points(points),
      dimensions(dimensions),
      data(data) { }
    
    t_value_ptr clustering(t_count num_centroids, 
                           t_value_ptr centroids,
                           t_cmeans_options options = t_cmeans_options());
                        
    t_value_ptr clustering(t_count num_centroids,
                           t_cmeans_options options = t_cmeans_options()) {
      t_value_ptr centroids = initial_centroids(num_centroids);
      return clustering(num_centroids, centroids, options);
    }

  private:
    t_value_ptr initial_centroids(t_id num_centroids);
    
    void update_centroids();
                                  
    void calculate_memberships();
                               
    t_value calculate_error() {
      t_value sum = 0;
      
      for(t_id i = 0; i < points; i++)
        for(t_id j = 0; j < num_centroids; j++)
          sum += MATRIX_CELL(memberships,i,j,num_centroids) * 
                 MATRIX_CELL(distances,i,j,num_centroids);
      
      return sum;
    }

    t_value_const_ptr data;
    t_value_ptr centroids;
    t_distance_ptr distances;
    t_membership_ptr memberships;
    t_count num_centroids, points, dimensions;
    t_cmeans_options options;
  };

}
}

#endif
