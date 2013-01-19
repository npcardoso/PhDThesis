#include "spectra_filter.h"
#include "spectra_iterator.h"

#include <iostream>


using namespace std;




int main(){
  t_spectra_filter filter;
  t_spectra_iterator it(4,4, &filter);
  filter.filter_component(1);
  filter.filter_component(4);
  filter.filter_transaction(1);
  filter.filter_transaction(4);

  while(it.next(true)){
    cout << it.get_component() << ", " << it.get_transaction() << endl; 
  }

}


