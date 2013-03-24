#ifndef __COMMON_IO_SPECTRA_H__
#define __COMMON_IO_SPECTRA_H__

#include <ostream>
#include "datastore/datastore.h"

namespace io {
namespace spectra {

  std::ostream & StateSpectra(std::ostream & out,
                              const t_datastore & ds);

}
}

#endif
