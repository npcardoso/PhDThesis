#include "cmeans.h"

#include <cmath>

namespace diagnosis {
namespace algorithms {

  t_value_ptr t_cmeans::get_spectra_centroids(const t_count_spectra & spectra) {
    t_count transactions = spectra.get_transaction_count();
    t_count components = spectra.get_component_count();
    assert(components == dimmensions);
    assert(transactions <= points);
    
    t_count last = components * transactions;
    t_value_ptr centroids( new t_value[last] );
    
    t_value faulty_transactions = spectra.get_error_count();
    t_value normal_transactions = transactions - faulty_transactions;
    assert(faulty_transactions != 0);
    assert(normal_transactions != 0);
    
    t_id centroid_row = 0;
    for(t_id t = 1; t <= transactions; t++) {
      if(spectra.is_error(t)) {
        for(t_id c = 1; c <= components; c++) {
          MATRIX_CELL(centroids,0,(c-1),components) += 
            t_value(spectra.get_activity(c,t)) / faulty_transactions;
        }
      }
      else {
        for(t_id c = 1; c <= components; c++) {
          MATRIX_CELL(centroids,1,(c-1),components) += 
            t_value(spectra.get_activity(c,t)) / normal_transactions;
        }
      }
    }
    return centroids;
  }

  t_value_ptr t_cmeans::initial_centroids(t_count num_centroids) {
    assert(num_centroids <= points);
    
    t_count last = num_centroids * dimensions;
    t_value_ptr centroids( new t_value[last] );
    
    for(t_id i = 0; i < last; i++)
      centroids[i] = data[i];
    
    return centroids;
  }
  
  t_value_ptr t_cmeans::clustering(t_count num_centroids, 
                                   t_value_ptr centroids,
                                   t_cmeans_configs configs) {
    this->num_centroids = num_centroids;
    this->centroids = centroids;
    this->configs = configs;
    this->distances = t_distance_ptr( new t_distance[points * num_centroids] );
    this->memberships = t_membership_ptr( new t_membership[points * num_centroids] );
    
    t_id iteration = 1;
    calculate_memberships();
    t_value error, old_error = calculate_error();
    
    bool stop = false;
    while(!stop && iteration++ < configs.num_iterations) {
      update_centroids();
      calculate_memberships();
      
      error = calculate_error();
      stop = std::abs(error - old_error) < configs.epsilon;
      old_error = error;
    }
    
    return this->memberships;
  }
  
  void t_cmeans::calculate_memberships() {
    
    for(t_id i = 0; i < points; i++)
      for(t_id j = 0; j < num_centroids; j++)
        MATRIX_CELL(distances,i,j,num_centroids) = 
          (*(configs.dist_function))(data, i, centroids, j, dimensions);

    t_value coef = 0;
    
    for(t_id i = 0; i < points; i++)
      for(t_id j = 0; j < num_centroids; j++) {
        coef = 0;
        for(t_id k = 0; k < num_centroids; k++)
          coef += pow( (MATRIX_CELL(distances,i,j,num_centroids) / 
                        MATRIX_CELL(distances,i,k,num_centroids)) , 
                      2.0 / (configs.m - 1.0) );
        
        if(std::isnan(coef))
          MATRIX_CELL(memberships,i,j,num_centroids) = 1;
        else
          MATRIX_CELL(memberships,i,j,num_centroids) = 1 / coef;
      }
  }
  
  void t_cmeans::update_centroids() {

    for(t_id i = configs.locked_centroids; i < num_centroids; i++)
      for(t_id j = 0; j < dimensions; j++)
       MATRIX_CELL(centroids,i,j,dimensions) = 0;
    
    t_value norm, p;
    for(t_id i = configs.locked_centroids; i < num_centroids; i++) {
      norm = 0;
      for(t_id j = 0; j < points; j++) {
        p = pow(MATRIX_CELL(memberships,j,i,num_centroids), configs.m);
        
        for(t_id k = 0; k < dimensions; k++)
          MATRIX_CELL(centroids,i,k,dimensions) += p * MATRIX_CELL(data,j,k,dimensions);
        
        norm += p;
      }
      
      for(t_id k = 0; k < dimensions; k++)
        MATRIX_CELL(centroids,i,k,dimensions) /= norm;
    }
  }

  t_value euclidean_distance(t_value_const_ptr data, 
                             t_id data_row,
                             t_value_const_ptr centroids,
                             t_id centroids_row,
                             t_count dimensions) {
    t_distance v, sum = 0;
    for(t_id i = 0; i < dimensions; i++) {
      v = MATRIX_CELL(data, data_row, i, dimensions) - 
          MATRIX_CELL(centroids, centroids_row, i, dimensions);
      sum += v*v;
    }
  
    return sqrt(sum);
  }

  t_distance manhattan_distance(t_value_const_ptr data, 
                                t_id data_row,
                                t_value_const_ptr centroids,
                                t_id centroids_row,
                                t_count dimensions) {
    t_distance v, sum = 0;
    for(t_id i = 0; i < dimensions; i++) {
      v = MATRIX_CELL(data, data_row, i, dimensions) - 
          MATRIX_CELL(centroids, centroids_row, i, dimensions);
      sum += std::fabs(v);
    }
  
    return sum;
  }

}
}
