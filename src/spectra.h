#ifndef __SPECTRA_H__
#define __SPECTRA_H__

#include "types.h"

#include <memory>
#include <set>

template <class T_ACTIVITY>
class t_spectra {
public:
  virtual bool is_error(t_transaction_id transaction) const = 0;
  
  virtual t_count get_transaction_count() const = 0;
  virtual t_count get_component_count() const = 0;

  virtual t_count get_count(t_component_id component,
                            t_transaction_id transaction) = 0;

  virtual T_ACTIVITY get_activity(t_component_id component,
                                  t_transaction_id transaction) = 0;

  virtual std::unique_ptr<t_component_id[]> get_ordering_buffer() const = 0;
};


template <class T_ACTIVITY>
class t_basic_spectra: t_spectra <T_ACTIVITY> {

  std::set <t_transaction_id> errors;
  t_component_id component_count;

public:

  inline virtual bool is_error(t_transaction_id transaction) const {
    return errors.count(transaction);
  }

  inline virtual void error(t_transaction_id transaction, bool set=true) {
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
