#ifndef __COMMON_IO_GRAPHVIZ_H__
#define __COMMON_IO_GRAPHVIZ_H__

#include <ostream>
#include "datastore/datastore.h"

namespace io {
namespace graphviz {
  std::ostream & graphviz(std::ostream & out, 
                          const t_datastore & ds);
}
}

#endif
