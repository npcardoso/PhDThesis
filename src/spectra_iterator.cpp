#include "spectra_iterator.h"

#include <cassert>

t_component_id t_spectra_iterator::next_component(t_component_id component) const {
  t_component_id next = 0;
  if(filter == NULL)
    next = component + 1;
  else
    next = filter->next_component(component);

  if(next > max_components)
    return 0;
  return next;
}

t_transaction_id t_spectra_iterator::next_transaction(t_transaction_id transaction) const {
  t_transaction_id next = 0;
  if(filter == NULL)
    next = transaction + 1;
  else
    next = filter->next_transaction(transaction);

  if(next > max_transactions)
    return 0;
  return next;
}

bool t_spectra_iterator::next_component() {
  if(!transaction)
    transaction = next_transaction(transaction);

  while (true) {
    t_component_id next_c = next_component(component);
    if(!next_c) {
      t_transaction_id next_t = next_transaction(transaction);
      if(!next_t)
        return false;
      transaction = next_t;
      component = 0;
    }
    else {
      component = next_c;

      assert(component <= max_components);
      assert(transaction <= max_transactions);
      return true;
    }
  }
}

bool t_spectra_iterator::next_transaction() {
  if(!component)
    component = next_component(component);

  while (true) {
    t_transaction_id next_t = next_transaction(transaction);
    if(!next_t) {
      t_component_id next_c = next_component(component);
      if(!next_c)
        return false;
      component = next_c;
      transaction = 0;
    } 
    else {
      transaction = next_t;

      assert(component <= max_components);
      assert(transaction <= max_transactions);
      return true;
    }
  }
}

bool t_spectra_iterator::next(bool transaction_oriented) {
  if(transaction_oriented) {
    if(!next_component())
      return next_transaction();
  }
  else {
    if(!next_transaction())
      return next_component();
  }
  return true;
}
