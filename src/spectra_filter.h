#ifndef __SPECTRA_FILTER_H__
#define __SPECTRA_FILTER_H__

#include "types.h"

#include <set>

class t_spectra_filter {
  std::set<t_component_id> components;
  std::set<t_transaction_id> transactions;

public:
  
  t_transaction_id next_transaction(t_transaction_id transaction) const;
  t_component_id next_component(t_component_id component) const;

  inline void filter_component(t_component_id component) {
    components.insert(component);
  }

  inline void filter_transaction(t_transaction_id transaction) {
    transactions.insert(transaction);
  }
};

#endif
