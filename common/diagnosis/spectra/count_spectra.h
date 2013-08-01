#ifndef __COUNT_SPECTRA_H__
#define __COUNT_SPECTRA_H__

#include "diagnosis/spectra_iterator.h"
#include "diagnosis/spectra.h"
#include "types.h"

#include <cstring>
#include <iomanip>

namespace diagnosis {

template <class T>
class t_count_spectra: public t_basic_spectra {
  typedef boost::shared_ptr<T[]> t_activity_ptr;
  t_activity_ptr activity;

public:

  inline t_count_spectra() {
  }
  
  inline t_count_spectra(t_count component_count, 
                         t_count transaction_count){
    set_element_count(component_count, transaction_count);

  }

  virtual T get_count(t_component_id component,
                            t_transaction_id transaction) const {
    return get_activity(component, transaction);
  }

  virtual void set_count(t_component_id component,
                         t_transaction_id transaction,
                         T count,
                         bool ignore_unknown_components=false) {
    assert(component > 0);
    assert(transaction > 0);

    if(component > get_component_count() ||
       transaction > get_transaction_count()) {
      if(!ignore_unknown_components)
        set_element_count(std::max(component, get_component_count()),
                          std::max(transaction, get_transaction_count()));
      else
        return;
    }

    activity[(transaction - 1) * get_component_count() + (component - 1)] = count;
  }

  virtual const T & get_activity(t_component_id component,
                                 t_transaction_id transaction) const {
    assert(component > 0);
    assert(component <= get_component_count());
    assert(transaction > 0);
    assert(transaction <= get_transaction_count());

    return activity[(transaction - 1) * get_component_count() + (component - 1)];
  }
  
  virtual void set_element_count(t_count component_count, 
                                 t_count transaction_count) {
    t_activity_ptr old_activity(activity);

    t_component_id max_component = std::min(component_count, get_component_count());
    t_component_id max_transaction = std::min(transaction_count, get_transaction_count());

    activity = t_activity_ptr(new T[component_count * transaction_count]);
    memset(activity.get(), 0, component_count * transaction_count * sizeof(T));

    for(t_component_id component = 0;
        component < max_component;
        component++)
      for(t_transaction_id transaction = 0;
          transaction < max_transaction;
          transaction++) {
        activity[transaction * component_count + component] =
        old_activity[transaction * get_component_count() + component];
      }

    t_basic_spectra::set_element_count(component_count,
                                       transaction_count);
  }

  t_transaction_id new_transaction() {
    set_element_count(get_component_count(), get_transaction_count() + 1);
    return get_transaction_count();
  }

  void hit(t_component_id component,
           t_transaction_id transaction,
           T count = 1,
           bool ignore_unknown_components=false) {
    assert(component > 0);
    assert(transaction > 0);

    if(component > get_component_count() ||
       transaction > get_transaction_count()) {
      if(!ignore_unknown_components)
        set_element_count(std::max(component, get_component_count()),
                          std::max(transaction, get_transaction_count()));
      else
        return;
    }

    activity[(transaction - 1) * get_component_count() + (component - 1)] += count;  
  }
  
  virtual std::ostream & print(std::ostream & out, 
                               const t_spectra_filter * filter = NULL) const {
    if(filter) {
      assert(filter->get_last_component() <= get_component_count());
      assert(filter->get_last_transaction() <= get_transaction_count());
    }

    t_spectra_iterator it(get_component_count(),
                        get_transaction_count(),
                        filter);
    out << "Tr |";
    while(it.next_component())
      out << std::setw(3) << it.get_component() << "|";
    out << "Err\n";

    while(it.next_transaction()){
      out << std::setw(3) << it.get_transaction() << "|";
      while(it.next_component())
        out << std::setw(3) << get_count(it.get_component(), it.get_transaction()) << "|";
      out << " " << is_error(it.get_transaction()) << "(" << get_error(it.get_transaction()) << ")\n";
    }
    return out;
  }
  
  virtual std::istream & read(std::istream & in) {
    t_count component_count, transaction_count;
    t_transaction_id transaction_offset = get_transaction_count();

    in >> component_count >> transaction_count;
    set_element_count(std::max(component_count, get_component_count()),
                      transaction_offset + transaction_count);

    t_count value;
    for(t_transaction_id transaction = 1;
        transaction <= transaction_count;
        transaction++) {
      for(t_component_id component = 1;
          component <= component_count;
          component++) {
        in >> value;
        if(value)
          hit(component, transaction_offset + transaction, true);
      }
      char result; //TODO: input for fuzzy error values
      in >> result;
      if(result == '1' || result == '-' || result == 'x' || result == 'X')
        error(transaction_offset + transaction);

    }

    return in;
  }
};

}

#endif
