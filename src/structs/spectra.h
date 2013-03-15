#ifndef __SPECTRA_H__
#define __SPECTRA_H__

#include "spectra_filter.h"
#include "spectra_iterator.h"

#include "../common/exceptions.h"
#include "../structs/candidate.h"
#include "../structs/types.h"

#include <cassert>
#include <iostream>
#include <vector>

template <class T_ACTIVITY>
class t_spectra {
public:
  virtual t_count get_error_count(const t_spectra_filter * filter = NULL) const = 0;
  virtual t_count get_transaction_count(const t_spectra_filter * filter = NULL) const = 0;
  virtual t_count get_component_count(const t_spectra_filter * filter = NULL) const = 0;

  virtual t_count get_count(t_component_id component,
                            t_transaction_id transaction) const = 0;

  virtual const T_ACTIVITY & get_activity(t_component_id component,
                                          t_transaction_id transaction) const = 0;

  virtual bool is_error(t_transaction_id transaction) const = 0;

  virtual bool is_candidate(const t_candidate & candidate,
                            const t_spectra_filter * filter = NULL) const {

    t_spectra_iterator it(get_component_count(),
                          get_transaction_count(),
                          filter);

    while(it.next_transaction()){
      bool hit = false;
      if(!is_error(it.get_transaction()))
        continue;

      t_candidate::iterator candidate_it = candidate.begin();
      while(candidate_it != candidate.end())
        if(get_count(*(candidate_it++), it.get_transaction())) {
          hit = true;
          break;
        }
      if(!hit)
        return false;
    }
    return true;
  }

  virtual t_order_buffer get_ordering_buffer(const t_spectra_filter * filter = NULL) const = 0;

  inline virtual std::ostream & print(std::ostream & out, 
                                      const t_spectra_filter * filter = NULL) const {
    return out << "Filtered Spectra output";
  }

  inline virtual std::istream & read (std::istream & in) {
    throw e_not_implemented();
  }
};

template <class T_ACTIVITY>
inline std::istream & operator >> (std::istream & in, t_spectra<T_ACTIVITY> & spectra){
  return spectra.read(in);
}

template <class T_ACTIVITY>
inline std::ostream & operator << (std::ostream & out, const t_spectra<T_ACTIVITY> & spectra){
  return spectra.print(out);
}


template <class T_ACTIVITY>
class t_basic_spectra: public t_spectra <T_ACTIVITY> {

  typedef std::vector <bool> t_errors;
  t_errors errors;
  t_count component_count;
  t_count transaction_count;


public:
  
  inline t_basic_spectra() {
    set_transaction_count(0);
    set_component_count(0);
  }
  
  inline t_basic_spectra(t_count component_count, 
                         t_count transaction_count) {
    set_transaction_count(transaction_count);
    this->transaction_count = transaction_count;
    set_component_count(component_count);
  }

  virtual t_count get_error_count(const t_spectra_filter * filter = NULL) const {
    //FIXME: Improve performance
    t_count filtered_errors = 0;
    if(filter)
      for(t_id i = 1; i <= get_component_count(); i++)
        if(is_error(i) && filter->is_transaction(i))
          filtered_errors++;
    
    return errors.size() - filtered_errors;
  }

  virtual t_count get_component_count(const t_spectra_filter * filter = NULL) const {
    assert(!filter || filter->get_last_component() <= component_count);
    if(filter)
      return component_count - filter->get_filtered_component_count();
    return component_count;
  }

  virtual t_count get_transaction_count(const t_spectra_filter * filter = NULL) const {
    assert(!filter || filter->get_last_transaction() <= transaction_count);
    if(filter)
      return transaction_count - filter->get_filtered_transaction_count();
    return transaction_count;
  }
  
  virtual void set_element_count(t_count component_count,
                                 t_count transaction_count) {
    this->component_count = component_count;
    
    this->transaction_count = transaction_count;

    errors.resize(transaction_count, false);
  }
  
  virtual void set_component_count(t_count component_count) {
    set_element_count(component_count, get_transaction_count());
  }
  
  virtual void set_transaction_count(t_count transaction_count) {
    set_element_count(get_component_count(), transaction_count);
  }


  inline virtual bool is_error(t_transaction_id transaction) const {
    assert(transaction > 0);
    assert(transaction <= transaction_count);

    return errors[transaction - 1];
  }

  inline virtual void error(t_transaction_id transaction, 
                            bool set=true) {
    assert(transaction > 0);
    assert(transaction <= transaction_count);

    errors[transaction - 1] = set;
  }

  virtual t_order_buffer get_ordering_buffer(const t_spectra_filter * filter = NULL) const {
    return t_order_buffer(new t_rank_element[get_component_count(filter)]);
  }
};

#endif
