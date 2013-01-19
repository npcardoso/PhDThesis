#include "count_spectra.h"

#include "spectra_iterator.h"

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

std::ostream & t_count_spectra::print(std::ostream & out, 
                                      const t_spectra_filter * filter) const {
  if(filter) {
    assert(filter->get_last_component() <= get_component_count());
    assert(filter->get_last_transaction() <= get_transaction_count());
  }

  t_spectra_iterator it(get_component_count(), 
                      get_transaction_count(), 
                      filter);
  out << "Tr |";
  while(it.next_component())
    out << std::setw(3) << it.get_component() << "|";
  out << "Err\n";
  
  while(it.next_transaction()){
    out << std::setw(3) << it.get_transaction() << "|";
    while(it.next_component())
      out << std::setw(3) << get_count(it.get_component(), it.get_transaction()) << "|";
    out << " " << is_error(it.get_transaction()) << "\n";
  }
  return out;
}

