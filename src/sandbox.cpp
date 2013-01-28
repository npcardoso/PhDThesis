#include "count_spectra.h"
#include "mhs.h"
#include "similarity.h"
#include "spectra.h"
#include "spectra_filter.h"
#include "spectra_iterator.h"
#include "trie.h"


#include <fstream>
#include <iostream>

using namespace std;



void test_trie_composite(const t_trie & trie, istream & in = cin) {
  t_candidate candidate;
  in >> candidate;
  if(trie.is_composite(candidate))
    puts("True");
  else
    puts("False");
}

void test_mhs(const char * filename = "in.mhs.txt") {
  ifstream f(filename);
  t_count_spectra count_spectra;
  t_mhs<t_count> mhs((t_ochiai<t_count>()));
  t_trie D;
  
  f >> count_spectra;
  cout << count_spectra;
  
  mhs.max_candidate_size = 3;
  mhs.calculate(count_spectra, D);
  D.print(cout);
  while(true) {
    t_candidate candidate;
    cin >> candidate;
    puts(count_spectra.is_candidate(candidate)?"True":"False"); 
  
    //test_trie_composite(D);
  }
}

void test_spectra() {
  t_spectra_filter filter;
  //t_spectra_iterator it(4,4);
  t_spectra_iterator it(3,3, &filter);
//  filter.filter_component(1);
  //filter.filter_component(2);
  //filter.filter_component(4);

  while(it.next(true)){
    cout << it.get_component() << ", " << it.get_transaction() << endl; 
  }
  cout << "--------\n";
  while(it.next(false)){
    cout << it.get_component() << ", " << it.get_transaction() << endl; 
  }

  t_count_spectra count_spectra(3,3);
  count_spectra.hit(1,1); 
  count_spectra.hit(2,2); 
  count_spectra.hit(3,3); 
  count_spectra.hit(1,3); 
  count_spectra.hit(3,1); 
  count_spectra.hit(3,2); 
  cout << count_spectra.get_error_count() << endl;
  count_spectra.error(1);
  cout << count_spectra.get_error_count() << endl;
  count_spectra.error(2);
  count_spectra.error(3);
  cout << count_spectra.get_error_count() << endl;
  cout << count_spectra.get_error_count(&filter) << endl;
  filter.filter_transaction(3);
  cout << count_spectra.get_error_count(&filter) << endl;
  filter.filter_transaction(1);
  cout << count_spectra.get_error_count(&filter) << endl;
  filter.filter_transaction(2);
  cout << count_spectra.get_error_count(&filter) << endl;
  cout << count_spectra;
  count_spectra.print(cout, &filter);
  t_ochiai<t_count> ochiai;
  unique_ptr<t_rank_element[]> order_buffer = count_spectra.get_ordering_buffer();

  ochiai.order(count_spectra, NULL, order_buffer.get());
  for(int i = 0; i < count_spectra.get_component_count(); i++){
    cout << order_buffer[i].get_component() << "," << order_buffer[i].get_value() << " " ;
  }
  cout << endl;
  ochiai.order(count_spectra, &filter, order_buffer.get());
  while(true) {
    t_candidate candidate;
    cin >> candidate;
    puts(count_spectra.is_candidate(candidate)?"True":"False"); 
  }
}

void test_trie() {
  t_trie trie;
  int option;
  t_candidate candidate;


  while (scanf("%d", &option) > 0){
    candidate.clear();
    puts("---------");
    switch(option) {
    case 1:
      while (scanf("%d", &option) > 0) {
        if(option < 1)
          break;
        candidate.insert(option);
      }
      trie.add(candidate);
      trie.print(cout);
      break;
    case 2:
      test_trie_composite(trie);
      break;
    default:
      return;
    }

  }

}

void sandbox(int argc, char ** argv) {
  cout << "Sandbox\n";
  test_spectra();
}

