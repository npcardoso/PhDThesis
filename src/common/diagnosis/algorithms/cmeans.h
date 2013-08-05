#ifndef __DIAGNOSIS_ALGORITHMS_CMEANS_H__
#define __DIAGNOSIS_ALGORITHMS_CMEANS_H__

#include "types.h"
#include "utils/boost.h"
#include "diagnosis/spectra/count_spectra.h"
#include "diagnosis/structs/membership.h"

//row-major order
#define MATRIX_CELL(x, r, c, nc) x[r * nc + c]

namespace diagnosis {
namespace algorithms {
  
  typedef double t_data;
  typedef boost::shared_ptr<t_data[]> t_data_ptr;
  typedef boost::shared_ptr<const t_data[]> t_data_const_ptr;
  
  typedef double t_distance;
  typedef boost::shared_ptr<t_distance[]> t_distance_ptr;
  
  typedef t_distance (*f_distance)(t_data_const_ptr data, 
                                   t_id data_row,
                                   t_data_const_ptr centroids,
                                   t_id centroids_row,
                                   t_count dimensions);

  t_distance euclidean_distance(t_data_const_ptr data, 
                                t_id data_row,
                                t_data_const_ptr centroids,
                                t_id centroids_row,
                                t_count dimensions);

  t_distance manhattan_distance(t_data_const_ptr data, 
                                t_id data_row,
                                t_data_const_ptr centroids,
                                t_id centroids_row,
                                t_count dimensions);

  class t_cmeans_configs {
  public:
    t_cmeans_configs() {
      initialize_defaults();
    }

    t_count num_iterations, locked_centroids;
    t_data m, epsilon;
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
    t_cmeans(t_count points, t_count dimensions, t_data_const_ptr data) : 
      points(points),
      dimensions(dimensions),
      data(data) { }
    
    t_cmeans(t_count_spectra & spectra): 
      points(spectra.get_transaction_count()),
      dimensions(spectra.get_component_count()) {
      t_data_ptr data_spectra(new t_data[points * dimensions]);

      for(t_id p = 1; p <= points; p++)
        for(t_id d = 1; d <= dimensions; d++) {
          MATRIX_CELL(data_spectra, (p-1), (d-1), dimensions) = spectra.get_count(d,p);
        }
      this->data = data_spectra;
    }
    
    structs::t_membership clustering(t_count num_centroids, 
                                     t_data_ptr centroids,
                                     t_cmeans_configs configs = t_cmeans_configs());
                        
    structs::t_membership clustering(t_count num_centroids,
                                     t_cmeans_configs configs = t_cmeans_configs()) {
      t_data_ptr centroids = initial_centroids(num_centroids);
      return clustering(num_centroids, centroids, configs);
    }
    
    structs::t_membership clustering(const t_count_spectra & spectra,
                                     t_cmeans_configs configs = t_cmeans_configs()) {
      t_data_ptr centroids = get_spectra_centroids(spectra);
      return clustering(2, centroids, configs);
    }
    
    t_data_ptr get_spectra_centroids(const t_count_spectra & spectra);
    
  private:
    t_data_ptr initial_centroids(t_id num_centroids);
    
    void update_centroids();
                                  
    void calculate_memberships();
                               
    t_data calculate_error() {
      t_data sum = 0;
      
      for(t_id i = 0; i < points; i++)
        for(t_id j = 0; j < num_centroids; j++)
          sum += memberships.get_membership(i,j,0) * 
                 MATRIX_CELL(distances,i,j,num_centroids);
      
      return sum;
    }

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
