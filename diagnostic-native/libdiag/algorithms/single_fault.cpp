#include "single_fault.h"

namespace diagnostic {
namespace algorithms {
void t_single_fault::operator () (const t_spectra & spectra,
                                  t_ret_type & D,
                                  const t_spectra_filter * filter) const {
    t_spectra_iterator it(spectra.get_component_count(),
                                   spectra.get_transaction_count(),
                                   filter);


    while (it.component.next()) {
        t_candidate c;
        c.insert(it.component.get());
        D.add(c);
    }
}
}
}
