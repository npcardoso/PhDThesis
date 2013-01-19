#include "count_spectra.h"

#include <iomanip>


t_count t_count_spectra::get_count(t_component_id component,
                                   t_transaction_id transaction) const {
  return get_activity(component, transaction);
}

const t_count & t_count_spectra::get_activity(t_component_id component,
                                              t_transaction_id transaction) const {
  assert(component > 0);
  assert(component <= get_component_count());
  assert(transaction > 0);
  assert(transaction <= get_transaction_count());

  return activity[(transaction - 1) * get_component_count() + (component - 1)];
}

void t_count_spectra::hit(t_component_id component,
                          t_transaction_id transaction,
                          t_count count) const {
  assert(component > 0);
  assert(component <= get_component_count());
  assert(transaction > 0);
  assert(transaction <= get_transaction_count());

  activity[(transaction - 1) * get_component_count() + (component - 1)] += count;

}

std::ostream & operator << (std::ostream & out, const t_count_spectra & spectra){

  for(t_component_id component = 1; 
      component <= spectra.get_component_count();
      component++)
    out << std::setw(3) << component << "|";
  out << "Err\n";

  for(t_transaction_id transaction = 1; 
      transaction <= spectra.get_transaction_count();
      transaction++) {
    for(t_component_id component = 1; 
        component <= spectra.get_component_count();
        component++)
      out << std::setw(3) << spectra.get_activity(component, transaction) << "|";
    out << " " <<  spectra.is_error(transaction) << "\n";
  }
  return out;
}
