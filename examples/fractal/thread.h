#pragma once
#include <sys/types.h>

class threadify_args_t {
  public:
    size_t level;
    size_t count;
    bool load;
    bool detach;
    
    inline threadify_args_t(size_t level, size_t count, bool load, bool detach=false) {
      this->level = level;
      this->count = count;
      this->load = load;
      this->detach = detach;
    }

    inline threadify_args_t * levelDown() const{
      return new threadify_args_t(level - 1, count, load);
    }
};

void *threadify(void * v);
