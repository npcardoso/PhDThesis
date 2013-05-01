#ifndef __SPECTRA_FILTER_H__
#define __SPECTRA_FILTER_H__

#include "../types.h"

#include <cassert>
#include <iostream>
#include <vector>

class t_spectra_filter {
  std::vector<t_component_id> f_component;
  std::vector<t_transaction_id> f_transaction;

  t_count filtered_component_count;
  t_count filtered_transaction_count;

public:
  
  t_spectra_filter();

  t_transaction_id next_transaction(t_transaction_id transaction) const;
  t_component_id next_component(t_component_id component) const;

  t_component_id get_last_component() const;
  t_transaction_id get_last_transaction() const;
  
  t_component_id get_filtered_component_count() const;
  t_transaction_id get_filtered_transaction_count() const;
  
  bool is_component(t_component_id component) const;
  bool is_transaction(t_transaction_id transaction) const;

  void filter_component(t_component_id component);
  void filter_transaction(t_transaction_id transaction);
  
};

#endif
