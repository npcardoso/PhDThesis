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

class t_ochiai_verb: public t_ochiai {
  t_count level;
public:
  t_ochiai_verb(t_count level):level(level){}
  virtual t_heuristic_value operator()(const t_count n[2][2]) const {
    std::cout << level << " ";
    return t_ochiai::operator()(n);
  }

};

void test_mhs(const char * filename = "in.mhs.txt") {
  ifstream f(filename);
  t_count_spectra count_spectra;
  t_mhs<t_count> mhs(new t_similarity<t_count>(new t_ochiai_verb(0)));
  t_trie D;

  mhs.set_heuristic(3, new t_similarity<t_count>(new t_ochiai_verb(3)));
  
  f >> count_spectra;
  cout << count_spectra;
  
//  mhs.max_candidate_size = 3;
  mhs.calculate(count_spectra, D);
  D.print(cout);
  while(true) {
    t_candidate candidate;
    cin >> candidate;
    puts(count_spectra.is_candidate(candidate)?"True":"False"); 
  
    //test_trie_composite(D);
  }
}
void test_parallel_similarity() {
  t_count_spectra count_spectra;
  example_count_spectra(count_spectra);
  
  t_parallel_similarity<t_count> ochiai(0, 2);
  
  t_order_buffer order_buffer = count_spectra.get_ordering_buffer();

  ochiai.order(count_spectra, order_buffer.get());
  for(int i = 0; i < count_spectra.get_component_count(); i++){
    cout << order_buffer[i].get_component() << "," << order_buffer[i].get_value() << " " ;
  }
  
  t_parallel_similarity<t_count> ochiai2(1, 2);
  
  ochiai2.order(count_spectra, order_buffer.get());
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
  t_similarity<t_count> ochiai;
  unique_ptr<t_rank_element[]> order_buffer = count_spectra.get_ordering_buffer();

  ochiai.order(count_spectra, order_buffer.get());
  for(int i = 0; i < count_spectra.get_component_count(); i++){
    cout << order_buffer[i].get_component() << "," << order_buffer[i].get_value() << " " ;
  }
  cout << endl;
  ochiai.order(count_spectra, order_buffer.get(), &filter);
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

