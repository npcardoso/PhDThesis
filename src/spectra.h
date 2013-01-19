#ifndef __SPECTRA_H__
#define __SPECTRA_H__

#include "types.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <set>

template <class T_ACTIVITY>
class t_spectra {
public:
  virtual t_count get_transaction_count() const = 0;
  virtual t_count get_component_count() const = 0;

  virtual t_count get_count(t_component_id component,
                            t_transaction_id transaction) const = 0;

  virtual const T_ACTIVITY & get_activity(t_component_id component,
                                          t_transaction_id transaction) const = 0;

  virtual bool is_error(t_transaction_id transaction) const = 0;
  
  virtual std::unique_ptr<t_component_id[]> get_ordering_buffer() const = 0;
};


template <class T_ACTIVITY>
class t_basic_spectra: public t_spectra <T_ACTIVITY> {

  std::set <t_transaction_id> errors;
  t_count component_count;
  t_count transaction_count;

public:
  inline t_basic_spectra(t_count component_count, 
                         t_count transaction_count) {
    assert(component_count > 0);
    assert(transaction_count > 0);

    this->component_count = component_count;
    this->transaction_count = transaction_count;
  }

  inline virtual t_count get_component_count() const {
    return component_count;
  }

  inline virtual t_count get_transaction_count() const {
    return transaction_count;
  }
  
  inline virtual bool is_error(t_transaction_id transaction) const {
    assert(transaction > 0);
    assert(transaction <= transaction_count);

    return errors.count(transaction);
  }

  inline virtual void error(t_transaction_id transaction, 
                            bool set=true) {
    assert(transaction > 0);
    assert(transaction <= transaction_count);

    if(set)
      errors.insert(transaction);
    else
      errors.erase(transaction);
  }

  inline virtual std::unique_ptr<t_component_id[]> get_ordering_buffer() const {
    return std::unique_ptr<t_component_id[]> (new t_component_id[component_count]);
  }
};

#endif
