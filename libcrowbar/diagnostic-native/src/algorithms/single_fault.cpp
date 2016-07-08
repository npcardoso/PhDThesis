#include "single_fault.h"

namespace diagnostic {
namespace algorithms {
void t_single_fault::operator () (const t_spectrum & spectrum,
                                  t_ret_type & D,
                                  const t_spectrum_filter * filter) const {
    t_spectrum_iterator it(spectrum.get_component_count(),
                                   spectrum.get_transaction_count(),
                                   filter);


    while (it.component.next()) {
        t_candidate c;
        c.insert(it.component.get());
        D.add(c);
    }
}
}
}
