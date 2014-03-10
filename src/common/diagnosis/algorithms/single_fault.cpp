#include "single_fault.h"
#include "diagnosis/structs/spectra_iterator.h"

namespace diagnosis {
namespace algorithms {
void t_single_fault::operator () (const structs::t_spectra & spectra,
                                  t_ret_type & D,
                                  const structs::t_spectra_filter * filter) const {
    structs::t_spectra_iterator it(spectra.get_component_count(),
                                   spectra.get_transaction_count(),
                                   filter);


    while (it.next_component()) {
        structs::t_candidate c;
        c.insert(it.get_component());
        D.add(c);
    }
}
}
}