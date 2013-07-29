#include "diagnosis/algorithms/cmeans.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace diagnosis;

void inline print(std::ostream & out, t_index nrows, t_index ncols, t_value_const_ptr p) {
  for (t_index i = 0; i < nrows; i++) {
    for (t_index j = 0; j < ncols; j++)
      out << MATRIX_CELL(p,i,j,ncols) << '\t';
    out << std::endl;
  }
}

int main() {
  srand (time(NULL));
  
  t_index number_points = 5;
  t_index dimension_point = 5;
  t_index num_centroids = 3;
  
  t_index size = number_points * dimension_point;
  t_value_ptr data(new t_value[number_points * dimension_point]);
  for(t_index i = 0; i < size; i++)
    data[i] = t_value(rand() % 100);
  
  std::cout << "Data" << std::endl;
  print(std::cout, number_points, dimension_point, data);
  
  algorithms::t_cmeans_options options;
  options.m = 1.6;
  
  algorithms::t_cmeans cm(number_points,dimension_point,data);
  t_value_ptr fuzzy_cluster = cm.clustering(num_centroids,options);
  
  std::cout << "Fuzzy cluster" << std::endl;
  print(std::cout, number_points, num_centroids, fuzzy_cluster);
  
  return 0;
}