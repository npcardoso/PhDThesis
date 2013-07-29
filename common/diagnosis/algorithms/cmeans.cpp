#include "cmeans.h"

#include <cmath>

namespace diagnosis {
namespace algorithms {

  t_value_ptr t_cmeans::initial_centroids(t_index num_centroids) {
    assert(num_centroids <= points);
    
    t_index last = num_centroids * dimensions;
    t_value_ptr centroids( new t_value[last] );
    
    for(t_index i = 0; i < last; i++)
      centroids[i] = data[i];
    
    return centroids;
  }
  
  t_value_ptr t_cmeans::clustering(t_index num_centroids, 
                                   t_value_ptr centroids,
                                   t_cmeans_options options) {
    this->num_centroids = num_centroids;
    this->centroids = centroids;
    this->options = options;
    this->distances = t_value_ptr( new t_value[points * num_centroids] );
    this->memberships = t_value_ptr( new t_value[points * num_centroids] );
    
    t_index iteration = 1;
    calculate_memberships();
    t_value error, old_error = calculate_error();
    
    bool stop = false;
    while(!stop && iteration++ < options.num_iterations) {
      update_centroids();
      calculate_memberships();
      
      error = calculate_error();
      stop = std::abs(error - old_error) < options.epsilon;
      old_error = error;
    }
    
    return this->memberships;
  }
  
  void t_cmeans::calculate_memberships() {
    
    for(t_index i = 0; i < points; i++)
      for(t_index j = 0; j < num_centroids; j++)
        MATRIX_CELL(distances,i,j,num_centroids) = 
          (*(options.dist_function))(data, i, centroids, j, dimensions);

    t_value coef = 0;
    
    for(t_index i = 0; i < points; i++)
      for(t_index j = 0; j < num_centroids; j++) {
        coef = 0;
        for(t_index k = 0; k < num_centroids; k++)
          coef += pow( (MATRIX_CELL(distances,i,j,num_centroids) / 
                        MATRIX_CELL(distances,i,k,num_centroids)) , 
                      2.0 / (options.m - 1.0) );
        
        if(std::isnan(coef))
          MATRIX_CELL(memberships,i,j,num_centroids) = 1;
        else
          MATRIX_CELL(memberships,i,j,num_centroids) = 1 / coef;
      }
  }
  
  void t_cmeans::update_centroids() {

    for(t_index i = options.locked_centroids; i < num_centroids; i++)
      for(t_index j = 0; j < dimensions; j++)
       MATRIX_CELL(centroids,i,j,dimensions) = 0;
    
    t_value norm, p;
    for(t_index i = options.locked_centroids; i < num_centroids; i++) {
      norm = 0;
      for(t_index j = 0; j < points; j++) {
        p = pow(MATRIX_CELL(memberships,j,i,num_centroids), options.m);
        
        for(t_index k = 0; k < dimensions; k++)
          MATRIX_CELL(centroids,i,k,dimensions) += p * MATRIX_CELL(data,j,k,dimensions);
        
        norm += p;
      }
      
      for(t_index k = 0; k < dimensions; k++)
        MATRIX_CELL(centroids,i,k,dimensions) /= norm;
    }
  }

  t_value euclidean_distance(t_value_const_ptr data, 
                                    t_index data_row,
                                    t_value_const_ptr centroids,
                                    t_index centroids_row,
                                    t_index dimensions) {
    t_value v, sum = 0;
    for(t_index i = 0; i < dimensions; i++) {
      v = MATRIX_CELL(data, data_row, i, dimensions) - 
          MATRIX_CELL(centroids, centroids_row, i, dimensions);
      sum += v*v;
    }
  
    return sqrt(sum);
  }

  t_value manhattan_distance(t_value_const_ptr data, 
                                    t_index data_row,
                                    t_value_const_ptr centroids,
                                    t_index centroids_row,
                                    t_index dimensions) {
    t_value v, sum = 0;
    for(t_index i = 0; i < dimensions; i++) {
      v = MATRIX_CELL(data, data_row, i, dimensions) - 
          MATRIX_CELL(centroids, centroids_row, i, dimensions);
      sum += std::fabs(v);
    }
  
    return sum;
  }
}
}