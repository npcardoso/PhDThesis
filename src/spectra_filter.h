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

  inline t_component_id get_last_component() const {
    if(components.size())
      return *components.rbegin();
    return 0;
  }
  
  inline t_component_id get_filtered_component_count() const {
    return components.size();
  }
  
  inline t_transaction_id get_last_transaction() const {
    if(transactions.size())
      return *transactions.rbegin();
    return 0;
  }
  
  inline t_transaction_id get_filtered_transaction_count() const {
    return transactions.size();
  }
  
  
  inline void filter_component(t_component_id component) {
    components.insert(component);
  }

  inline void filter_transaction(t_transaction_id transaction) {
    transactions.insert(transaction);
  }

};

#endif
