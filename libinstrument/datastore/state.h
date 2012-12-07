#pragma once

#include "../types.h"

struct State {
public:
  unsigned char * data;
  size_t * offset_end;
  size_t n_vars;

public:
  State();
  ~State();

  void readVariable(const void * var,
                    size_t bytes);

  inline size_t dataSize() const {
    if(n_vars)
      return offset_end[n_vars - 1];
    else
      return 0;
  }

  //Deprecated
  inline size_t rawSize() const {
    return dataSize();
  }

  inline size_t size() const {
    return sizeof(State) + sizeof(size_t) * n_vars + dataSize();
  }
};
