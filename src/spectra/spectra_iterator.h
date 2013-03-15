#ifndef __SPECTRA_ITERATOR_H__
#define __SPECTRA_ITERATOR_H__

#include "spectra_filter.h"

#include "../types.h"

#include <cstdlib>

class t_spectra_iterator {
  t_count max_components;
  t_count max_transactions;
  
  t_transaction_id transaction;
  t_component_id component;
  
  const t_spectra_filter * filter;
  
  t_component_id _next_component(t_component_id component) const;
  
  t_transaction_id _next_transaction(t_transaction_id transaction) const;

public:
  inline t_spectra_iterator(t_count max_components, 
                            t_count max_transactions, 
                            const t_spectra_filter * filter = NULL) {
    this->max_components = max_components;
    this->max_transactions = max_transactions;
    this->filter = filter;
    component = 0;
    transaction = 0;
  }

  
  t_component_id next_component(bool wrap=false);

  t_transaction_id next_transaction(bool wrap=false);

  bool next(bool transaction_oriented = false);

  inline t_transaction_id get_transaction() const {
    return transaction;
  }

  inline t_component_id get_component() const {
    return component;
  }
  
  void set_transaction(t_transaction_id transaction);
  
  void set_component(t_component_id component);
};
#endif
