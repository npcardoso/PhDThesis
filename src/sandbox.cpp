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

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;


void example_count_spectra(t_count_spectra & count_spectra, t_count component_count = 3, t_count transaction_count = 3, float activity=0.3, float error=0.5) {
  count_spectra.set_element_count(component_count, transaction_count);
  for(t_transaction_id t = 1; t <= transaction_count; t++) {
    for(t_component_id c = 1; c <= component_count; c++)
      if(rand() < RAND_MAX * activity)
        count_spectra.hit(c, t);
      
    if(rand() < RAND_MAX * error)
      count_spectra.error(t);
  
  }
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

void test_mhs_combine(const t_count_spectra & count_spectra) {
  t_heuristic<t_count> heuristic;
  heuristic.push(new t_filter_ochiai<t_count>());
  heuristic.push(new t_filter_sort<t_count>());
  
  t_mhs<t_count> mhs(heuristic);

  t_trie D, D_first, D_second;
  t_spectra_filter filter_first, filter_second;
  
  for(t_component_id component = 0;
      component < count_spectra.get_component_count();
      component++)
    if(component > count_spectra.get_component_count() / 2)
      filter_first.filter_transaction(component+1);
    else
      filter_second.filter_transaction(component+1);

  cout << count_spectra << endl;
  mhs.calculate(count_spectra, D_first, &filter_first);  
  mhs.calculate(count_spectra, D_second, &filter_second);  

  mhs.combine(count_spectra, D, D_first, D_second, filter_first, filter_second);
  cout << D <<endl;
  
}

void test_mhs_update(const t_count_spectra & count_spectra) {
  t_heuristic<t_count> heuristic;
  heuristic.push(new t_filter_ochiai<t_count>());
  heuristic.push(new t_filter_sort<t_count>());
  t_mhs<t_count> mhs(heuristic);

  t_trie D, D_first;

  t_spectra_filter filter_first, filter_second;
  
  for(t_component_id component = 0;
      component < count_spectra.get_component_count();
      component++)
    if(component > 0)// * count_spectra.get_component_count() / 2)
      filter_first.filter_transaction(component+1);
    else
      filter_second.filter_transaction(component+1);

  cout << count_spectra;
  mhs.calculate(count_spectra, D_first, &filter_first);  
//  cout << D_first;

  mhs.update(count_spectra, D, D_first, filter_second);
  cout << D;
  
}


void test_spectra() {
  t_spectra_filter filter;
  //t_spectra_iterator it(4,4);
  t_spectra_iterator it(5,5, &filter);
  filter.filter_component(2);
  filter.filter_component(1);
  filter.filter_transaction(3);
  filter.filter_transaction(1);

  while(it.next(true)){
    cout << it.get_component() << ", " << it.get_transaction() << endl; 
  }
  cout << "--------\n";
  while(it.next(false)){
    cout << it.get_component() << ", " << it.get_transaction() << endl; 
  }

  t_count_spectra count_spectra;
  example_count_spectra(count_spectra, 5, 5, 0.5,1);
  cout << count_spectra;
  count_spectra.print(cout, &filter);
  return;
  
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
  t_count_spectra count_spectra;
  srand(time(NULL));
//  cin >> count_spectra;
  test_spectra();
}

