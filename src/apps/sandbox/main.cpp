#include "main.h"

#include "../../structs/component.h"

#include <iostream>

int main_sandbox() {
  std::cout << "Sandbox" << std::endl;

  t_goodness_model g;
  t_density_kernel_single * pass = new t_density_kernel_single(new t_kernel_gaussian(), 1);
  pass->push_back(0);

  t_density_kernel_single * fail = new t_density_kernel_single(new t_kernel_uniform(), 1);
  fail->push_back(0);

  g.label(pass, 2);
  g.label(fail, 1);

  std::cout << "g(0, -1) = " << g.probability(0, -1) << std::endl;
  std::cout << "g(0, -0.5) = " << g.probability(0, -0.5) << std::endl;
  std::cout << "g(0, 0.5) = " << g.probability(0, 0.5) << std::endl;
  std::cout << "g(0, 1) = " << g.probability(0, 1) << std::endl;
  std::cout << "g(0, 1.5) = " << g.probability(0, 1.5) << std::endl;

  return 0;
}

