#include "algorithms/mhs.h"
#include "common/trie.h"
#include "heuristic/cutoff.h"
#include "heuristic/parallelization.h"
#include "heuristic/similarity.h"
#include "heuristic/sort.h"

#include "spectra/count_spectra.h"
#include "spectra/spectra.h"
#include "spectra/spectra_filter.h"
#include "spectra/spectra_iterator.h"


#include <fstream>
#include <iostream>

using namespace std;


void example_count_spectra(t_count_spectra & count_spectra) {
  count_spectra.set_element_count(3,3);
  count_spectra.hit(1,1); 
  count_spectra.hit(2,2); 
  count_spectra.hit(3,3); 
  count_spectra.hit(1,3); 
  count_spectra.hit(3,1); 
  count_spectra.hit(3,2); 
  count_spectra.error(1);
  count_spectra.error(2);
  count_spectra.error(3);
}

void test_parallel_similarity() {
  t_count_spectra count_spectra;
  example_count_spectra(count_spectra);
  
  t_heuristic<t_count> heuristic;
  heuristic.push(new t_filter_ochiai<t_count>());
  heuristic.push(new t_filter_sort<t_count>());
  heuristic.push(new t_filter_divide<t_count>(0,2));

  t_order_buffer order_buffer = count_spectra.get_ordering_buffer();

  heuristic(count_spectra, order_buffer.get());
  for(int i = 0; i < count_spectra.get_component_count(); i++){
    cout << order_buffer[i].get_component() << "," << order_buffer[i].get_value() << " " ;
  }
  cout << std::endl;
  heuristic = t_heuristic<t_count>();
  heuristic.push(new t_filter_ochiai<t_count>());
  heuristic.push(new t_filter_sort<t_count>());
  heuristic.push(new t_filter_divide<t_count>(1,2));
  heuristic.push(new t_filter_cutoff<t_count>(0,0.7));
  
  heuristic(count_spectra, order_buffer.get());
  for(int i = 0; i < count_spectra.get_component_count(); i++){
    cout << order_buffer[i].get_component() << "," << order_buffer[i].get_value() << " " ;
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

  t_count_spectra count_spectra;
  example_count_spectra(count_spectra);
  cout << count_spectra;
  count_spectra.print(cout, &filter);
  
  t_heuristic<t_count> heuristic;
  heuristic.push(new t_filter_ochiai<t_count>());
  heuristic.push(new t_filter_sort<t_count>());
  
  unique_ptr<t_rank_element[]> order_buffer = count_spectra.get_ordering_buffer();

  heuristic(count_spectra, order_buffer.get());
  for(int i = 0; i < count_spectra.get_component_count(); i++){
    cout << order_buffer[i].get_component() << "," << order_buffer[i].get_value() << " " ;
  }
  cout << endl;
  heuristic(count_spectra, order_buffer.get(), &filter);
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
      cin >> candidate;
      trie.add(candidate);
      trie.print(cout);
      cout <<"Size: "<< trie.size()<<std::endl;
      break;
    case 2:
      cin >> candidate;
      cout << (trie.is_composite(candidate))<<endl;
      break;
    default:
      return;
    }

  }

}

void sandbox(int argc, char ** argv) {
  cout << "Sandbox\n";
  test_parallel_similarity();
}

