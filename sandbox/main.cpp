#include "utils/debug.h"
#include <iostream>
#include <string>

int main() {
  try {
  }
  catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
