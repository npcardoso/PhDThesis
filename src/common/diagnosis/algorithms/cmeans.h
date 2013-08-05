#ifndef __DIAGNOSIS_ALGORITHMS_CMEANS_H__
#define __DIAGNOSIS_ALGORITHMS_CMEANS_H__

#include "types.h"
#include "utils/boost.h"
#include "diagnosis/spectra/count_spectra.h"

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

  class t_cmeans_configs {
  public:
    t_cmeans_configs() {
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
    
    t_cmeans(t_count_spectra & spectra): 
      points(spectra.get_transaction_count()),
      dimensions(spectra.get_component_count()) {
      t_value_ptr data_spectra(new t_value[points * dimensions]);

      for(t_id p = 1; p <= points; p++)
        for(t_id d = 1; d <= dimensions; d++) {
          MATRIX_CELL(data_spectra, (p-1), (d-1), dimensions) = spectra.get_activity(d,p);
        }
      this->data = data_spectra;
    }
    
    t_value_ptr clustering(t_count num_centroids, 
                           t_value_ptr centroids,
                           t_cmeans_configs configs = t_cmeans_configs());
                        
    t_value_ptr clustering(t_count num_centroids,
                           t_cmeans_configs configs = t_cmeans_configs()) {
      t_value_ptr centroids = initial_centroids(num_centroids);
      return clustering(num_centroids, centroids, configs);
    }
    
    t_value_ptr clustering(const t_count_spectra & spectra,
                           t_cmeans_configs configs = t_cmeans_configs()) {
      t_value_ptr centroids = get_spectra_centroids(spectra);
      return clustering(2, centroids, configs);
    }
    
    t_value_ptr get_spectra_centroids(const t_count_spectra & spectra);
    
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
    t_cmeans_configs configs;
  };

}
}

#endif
