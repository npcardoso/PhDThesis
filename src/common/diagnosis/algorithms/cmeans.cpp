#include "cmeans.h"

#include <cmath>

namespace diagnosis {
namespace algorithms {

  t_data_ptr t_cmeans::get_spectra_centroids(const t_count_spectra & spectra) {
    t_count transactions = spectra.get_transaction_count();
    t_count components = spectra.get_component_count();
    assert(components == dimmensions);
    assert(transactions <= points);
    
    t_count last = components * transactions;
    t_data_ptr centroids( new t_data[last] );
    for(t_id i = 0; i < last; i++)
      centroids[i] = 0;
    
    t_data faulty_transactions = spectra.get_error_count();
    t_data normal_transactions = transactions - faulty_transactions;
    assert(faulty_transactions != 0);
    assert(normal_transactions != 0);
    
    t_id centroid_row = 0;
    for(t_id t = 1; t <= transactions; t++) {
      if(spectra.is_error(t)) {
        for(t_id c = 1; c <= components; c++) {
          MATRIX_CELL(centroids,0,(c-1),components) += 
            t_data(spectra.get_count(c,t)) / faulty_transactions;
        }
      }
      else {
        for(t_id c = 1; c <= components; c++) {
          MATRIX_CELL(centroids,1,(c-1),components) += 
            t_data(spectra.get_count(c,t)) / normal_transactions;
        }
      }
    }
    return centroids;
  }

  t_data_ptr t_cmeans::initial_centroids(t_count num_centroids) {
    assert(num_centroids <= points);
    
    t_count last = num_centroids * dimensions;
    t_data_ptr centroids( new t_data[last] );
    
    for(t_id i = 0; i < last; i++)
      centroids[i] = data[i];
    
    return centroids;
  }
  
  structs::t_membership t_cmeans::clustering(t_count num_centroids, 
                                             t_data_ptr centroids,
                                             t_cmeans_configs configs) {
    this->num_centroids = num_centroids;
    this->centroids = centroids;
    this->configs = configs;
    this->distances = t_distance_ptr( new t_distance[points * num_centroids] );
    this->memberships = structs::t_membership(points, num_centroids);
    
    t_id iteration = 1;
    calculate_memberships();
    t_data error, old_error = calculate_error();
    
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

    t_data coef = 0;
    
    for(t_id i = 0; i < points; i++)
      for(t_id j = 0; j < num_centroids; j++) {
        coef = 0;
        for(t_id k = 0; k < num_centroids; k++)
          coef += pow( (MATRIX_CELL(distances,i,j,num_centroids) / 
                        MATRIX_CELL(distances,i,k,num_centroids)) , 
                      2.0 / (configs.m - 1.0) );
        
        if(std::isnan(coef))
          memberships.set_membership(i,j,1.0,0);
        else
          memberships.set_membership(i,j,(1.0 / coef),0);
      }
  }
  
  void t_cmeans::update_centroids() {

    for(t_id i = configs.locked_centroids; i < num_centroids; i++)
      for(t_id j = 0; j < dimensions; j++)
       MATRIX_CELL(centroids,i,j,dimensions) = 0;
    
    t_data norm, p;
    for(t_id i = configs.locked_centroids; i < num_centroids; i++) {
      norm = 0;
      for(t_id j = 0; j < points; j++) {
        p = pow(memberships.get_membership(j,i,0), configs.m);
        
        for(t_id k = 0; k < dimensions; k++)
          MATRIX_CELL(centroids,i,k,dimensions) += p * MATRIX_CELL(data,j,k,dimensions);
        
        norm += p;
      }
      
      for(t_id k = 0; k < dimensions; k++)
        MATRIX_CELL(centroids,i,k,dimensions) /= norm;
    }
  }

  t_distance euclidean_distance(t_data_const_ptr data, 
                                t_id data_row,
                                t_data_const_ptr centroids,
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

  t_distance manhattan_distance(t_data_const_ptr data, 
                                t_id data_row,
                                t_data_const_ptr centroids,
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
