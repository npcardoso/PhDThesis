#include "single_fault.h"

namespace diagnosis {
namespace algorithms {
void t_single_fault::operator () (const structs::t_spectra & spectra,
                                  t_ret_type & D,
                                  const structs::t_spectra_filter * filter) const {
    structs::t_spectra_iterator it(spectra.get_component_count(),
                                   spectra.get_transaction_count(),
                                   filter);


    while (it.component.next()) {
        structs::t_candidate c;
        c.insert(it.component.get());
        D.add(c);
    }
}
}
}
