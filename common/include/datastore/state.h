#pragma once

#include "../types.h"

struct t_state {
public:
  unsigned char * data;
  size_t * offset_end;
  size_t n_vars;

public:
  t_state();
  ~t_state();

  void read_variable(const void * var,
                     size_t bytes);

  inline size_t data_size() const {
    if(n_vars)
      return offset_end[n_vars - 1];
    else
      return 0;
  }

  inline size_t size() const {
    return sizeof(size_t) * n_vars + data_size();
  }
};
