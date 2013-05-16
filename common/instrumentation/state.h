#ifndef __COMMON_INSTRUMENTATION_STATE_H__
#define __COMMON_INSTRUMENTATION_STATE_H__

#include "types.h"

namespace instrumentation {

struct t_state {
public:
  unsigned char * data;
  t_count * offset_end;
  t_count n_vars;

public:
  t_state();
  t_state(const t_state & s);
  ~t_state();

  void read_variable(const void * var,
                     t_count bytes);

  inline t_count data_size() const {
    if(n_vars)
      return offset_end[n_vars - 1];
    else
      return 0;
  }

  inline t_count size() const {
    return sizeof(t_count) * n_vars + data_size();
  }
};
}

#endif
