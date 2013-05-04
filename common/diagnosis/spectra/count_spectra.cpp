#include "count_spectra.h"

#include "spectra_iterator.h"

#include <cstring>
#include <iomanip>


void t_count_spectra::set_element_count(t_count component_count, 
                                        t_count transaction_count) {
  t_activity_ptr old_activity(activity);

  t_component_id max_component = std::min(component_count, get_component_count());
  t_component_id max_transaction = std::min(transaction_count, get_transaction_count());

  activity = t_activity_ptr(new t_count[component_count * transaction_count]);
  memset(activity.get(), 0, component_count * transaction_count * sizeof(t_count));
  
  for(t_component_id component = 0; 
      component < max_component; 
      component++)
    for(t_transaction_id transaction = 0; 
        transaction < max_transaction; 
        transaction++) {
      activity[transaction * component_count + component] = 
      old_activity[transaction * get_component_count() + component];
    }

  t_basic_spectra::set_element_count(component_count, 
                                     transaction_count);
}

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

t_transaction_id t_count_spectra::new_transaction() {
  set_element_count(get_component_count(), get_transaction_count() + 1);
  return get_transaction_count();
}

void t_count_spectra::hit(t_component_id component,
                          t_transaction_id transaction,
                          bool ignore_unknown_components,
                          t_count count) {
  assert(component > 0);
  assert(transaction > 0);
  
  if(component > get_component_count() ||
     transaction > get_transaction_count()) {
    if(!ignore_unknown_components) 
      set_element_count(std::max(component, get_component_count()), 
                        std::max(transaction, get_transaction_count()));
    else 
      return;
  } 

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

std::istream & t_count_spectra::read(std::istream & in) {
  t_count component_count, transaction_count;
  t_transaction_id transaction_offset = get_transaction_count();

  in >> component_count >> transaction_count;
  set_element_count(std::max(component_count, get_component_count()), 
                    transaction_offset + transaction_count);

  t_count value;
  for(t_transaction_id transaction = 1;
      transaction <= transaction_count;
      transaction++) {
    for(t_component_id component = 1;
        component <= component_count;
        component++) {
      in >> value;
      if(value)
        hit(component, transaction_offset + transaction, true);
    }
    char result;
    in >> result;
    if(result == '1' || result == '-' || result == 'x' || result == 'X')
      error(transaction_offset + transaction);
  
  }

  return in;
}
