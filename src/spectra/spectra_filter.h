#ifndef __SPECTRA_FILTER_H__
#define __SPECTRA_FILTER_H__

#include "../common/types.h"

#include <cassert>
#include <iostream>
#include <vector>

class t_spectra_filter {
  std::vector<t_component_id> f_component;
  std::vector<t_transaction_id> f_transaction;

  t_count filtered_component_count;
  t_count filtered_transaction_count;

public:
  
  inline t_spectra_filter(){
    filtered_component_count = 0;
    filtered_transaction_count = 0;
  }

  inline t_transaction_id next_transaction(t_transaction_id transaction) const {
    if(transaction >= f_transaction.size())
      return transaction + 1;
    else
      return f_transaction[transaction];
  }

  inline t_component_id next_component(t_component_id component) const {
    if(component >= f_component.size())
      return component + 1;
    else
      return f_component[component];
  }

  inline t_component_id get_last_component() const {
    return f_component.size()?(f_component.size() - 1):0UL;
  }
  
  inline t_transaction_id get_last_transaction() const {
    return f_transaction.size()?(f_transaction.size() - 1):0UL;
  }
  
  inline t_component_id get_filtered_component_count() const {
    return filtered_component_count;
  }
  
  inline t_transaction_id get_filtered_transaction_count() const {
    return filtered_transaction_count;
  }
  
  inline bool is_component(t_component_id component) const {
    assert(component > 0);
    if(component >= f_component.size())
      return false;
    return f_component[component - 1] != component;
  }

  inline bool is_transaction(t_transaction_id transaction) const {
    assert(transaction > 0);
    if(transaction >= f_transaction.size())
      return false;
    return f_transaction[transaction - 1] != transaction;
  }

  inline void filter_component(t_component_id component) {
    assert(component > 0);
    
    if(is_component(component))
      return;
    
    filtered_component_count++;
    
    if(component >= f_component.size()){
      t_count old_size = f_component.size();
      f_component.resize(component + 1);
      while(old_size++ < f_component.size())
        f_component[old_size - 1] = old_size;  
    }

    t_component_id next = f_component[component];
    t_component_id i = component;
    while(i--)
      if(f_component[i] == component)
        f_component[i] = next;
  }

  inline void filter_transaction(t_transaction_id transaction) {
    assert(transaction > 0);
    if(is_transaction(transaction))
      return;
  
    filtered_transaction_count++;

    if(transaction >= f_transaction.size()){
      t_count old_size = f_transaction.size();
      f_transaction.resize(transaction + 1);
      while(old_size++ < f_transaction.size())
        f_transaction[old_size - 1] = old_size;  
    }

    t_transaction_id next = f_transaction[transaction];
    t_transaction_id i = transaction;
    while(i--)
      if(f_transaction[i] == transaction)
        f_transaction[i] = next;
  }
  
};

#endif
