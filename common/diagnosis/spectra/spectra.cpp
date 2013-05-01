#include "spectra.h"

bool t_spectra::is_candidate(const t_candidate & candidate,
                             const t_spectra_filter * filter) const {

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

t_order_buffer t_spectra::get_ordering_buffer(const t_spectra_filter * filter) const {
  return t_order_buffer(new t_rank_element[get_component_count(filter)]);
}

std::ostream & t_spectra::print(std::ostream & out, 
                                const t_spectra_filter * filter) const {
  return out << "Filtered Spectra output";
}

std::istream & t_spectra::read (std::istream & in) {
  throw e_not_implemented();
}

std::istream & operator >> (std::istream & in, t_spectra & spectra){
  return spectra.read(in);
}

std::ostream & operator << (std::ostream & out, const t_spectra & spectra){
  return spectra.print(out);
}


t_basic_spectra::t_basic_spectra() {
  set_transaction_count(0);
  set_component_count(0);
}

t_basic_spectra::t_basic_spectra(t_count component_count, 
                                        t_count transaction_count) {
  set_transaction_count(transaction_count);
  this->transaction_count = transaction_count;
  set_component_count(component_count);
}

t_count t_basic_spectra::get_error_count(const t_spectra_filter * filter) const {
  //FIXME: Improve performance
  t_count filtered_errors = 0;
  if(filter)
    for(t_id i = 1; i <= get_component_count(); i++)
      if(is_error(i) && filter->is_transaction(i))
        filtered_errors++;

  return errors.size() - filtered_errors;
}

t_count t_basic_spectra::get_component_count(const t_spectra_filter * filter) const {
  assert(!filter || filter->get_last_component() <= component_count);
  if(filter)
    return component_count - filter->get_filtered_component_count();
  return component_count;
}

t_count t_basic_spectra::get_transaction_count(const t_spectra_filter * filter) const {
  assert(!filter || filter->get_last_transaction() <= transaction_count);
  if(filter)
    return transaction_count - filter->get_filtered_transaction_count();
  return transaction_count;
}

void t_basic_spectra::set_element_count(t_count component_count,
                                                t_count transaction_count) {
  this->component_count = component_count;

  this->transaction_count = transaction_count;

  errors.resize(transaction_count, false);
}

void t_basic_spectra::set_component_count(t_count component_count) {
  set_element_count(component_count, get_transaction_count());
}

void t_basic_spectra::set_transaction_count(t_count transaction_count) {
  set_element_count(get_component_count(), transaction_count);
}


bool t_basic_spectra::is_error(t_transaction_id transaction) const {
  assert(transaction > 0);
  assert(transaction <= transaction_count);

  return errors[transaction - 1];
}

void t_basic_spectra::error(t_transaction_id transaction, 
                                           bool set) {
  assert(transaction > 0);
  assert(transaction <= transaction_count);

  errors[transaction - 1] = set;
}
