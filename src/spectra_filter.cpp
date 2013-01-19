#include "spectra_filter.h"

#include "utils.h"

#include <algorithm>

using namespace std;

t_transaction_id t_spectra_filter::next_transaction(t_transaction_id transaction) const {
  set <t_transaction_id>::iterator it;
  t_transaction_id last = transaction;

  it = upper_bound(iterators(transactions), transaction);

  while (it != transactions.end() &&
         *it <= last + 1)
    last = *(it++);

  return last + 1;
}

t_component_id t_spectra_filter::next_component(t_component_id component) const {
  set <t_component_id>::iterator it;
  t_component_id last = component;

  it = upper_bound(iterators(components), component);

  while (it != components.end() && 
         *it <= last + 1)
    last = *(it++);

  return last + 1;
}
