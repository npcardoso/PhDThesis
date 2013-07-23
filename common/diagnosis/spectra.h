#ifndef __DIAGNOSIS_SPECTRA_H__
#define __DIAGNOSIS_SPECTRA_H__

#include "diagnosis/candidate.h"
#include "diagnosis/spectra_filter.h"
#include "diagnosis/spectra_iterator.h"

#include "exceptions.h"
#include "types.h"

#include <cassert>
#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace diagnosis {

class t_spectra {
public:

  virtual t_count get_error_count(const t_spectra_filter * filter = NULL) const = 0;
  virtual t_count get_transaction_count(const t_spectra_filter * filter = NULL) const = 0;
  virtual t_count get_component_count(const t_spectra_filter * filter = NULL) const = 0;

  virtual t_count get_count(t_component_id component,
                            t_transaction_id transaction) const = 0;

//  virtual const T_ACTIVITY & get_activity(t_component_id component,
//                                          t_transaction_id transaction) const = 0;

  virtual bool is_error(t_transaction_id transaction) const = 0;
  virtual t_error get_error(t_transaction_id transaction) const = 0;

  virtual bool is_candidate(const t_candidate & candidate,
                            const t_spectra_filter * filter = NULL) const;

  virtual std::ostream & print(std::ostream & out,
                                      const t_spectra_filter * filter = NULL) const;

  virtual std::istream & read (std::istream & in);
};

std::istream & operator >> (std::istream & in, t_spectra & spectra);
std::ostream & operator << (std::ostream & out, const t_spectra & spectra);

class t_basic_spectra: public t_spectra {

  typedef std::vector <t_error> t_errors;
  t_errors errors;
  t_count component_count;
  t_count transaction_count;


public:

  t_basic_spectra();

  t_basic_spectra(t_count component_count,
                         t_count transaction_count);

  virtual t_count get_error_count(const t_spectra_filter * filter = NULL) const;
  virtual t_count get_component_count(const t_spectra_filter * filter = NULL) const;
  virtual t_count get_transaction_count(const t_spectra_filter * filter = NULL) const;

  virtual void set_element_count(t_count component_count,
                                 t_count transaction_count);
  virtual void set_component_count(t_count component_count);
  virtual void set_transaction_count(t_count transaction_count);


  virtual t_error get_error(t_transaction_id transaction) const;
  virtual bool is_error(t_transaction_id transaction) const;
  virtual void error(t_transaction_id transaction,
                     t_error set=1);
};

}
#endif
