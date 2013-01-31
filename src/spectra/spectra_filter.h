#ifndef __SPECTRA_FILTER_H__
#define __SPECTRA_FILTER_H__

#include "../common/types.h"

#include <cassert>
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
  
  inline bool is_component(t_component_id component) const {
    assert(component > 0);
    return components.count(component);
  }

  inline bool is_transaction(t_transaction_id transaction) const {
    assert(transaction > 0);
    return transactions.count(transaction);
  }

  inline void filter_component(t_component_id component) {
    assert(component > 0);
    components.insert(component);
  }

  inline void filter_transaction(t_transaction_id transaction) {
    assert(transaction > 0);
    transactions.insert(transaction);
  }
  
  inline void unfilter_component(t_component_id component) {
    assert(component > 0);
    components.erase(component);
  }

  inline void unfilter_transaction(t_transaction_id transaction) {
    assert(transaction > 0);
    transactions.erase(transaction);
  }

};

#endif
