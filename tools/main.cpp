#include "iscandidate/main.h"
#include "trie/main.h"
#include "mhs/main.h"
#include "sandbox/main.h"

#include "../utils/utils.h"
#include "../types.h"


#include <cstring>
#include <cstdio>

#include <iostream>

void print_help(char * self) {
  std::cerr << "Usage: " << self << " <mhs | trie | sandbox> [options]\n";
}

int main(int argc, char ** argv){
  int ret = 0;

  if(argc == 1) {
    print_help(*argv);
    return 1;
  }
  else {
    if(strcmp("trie", argv[1]) == 0) { // Trie mode
      t_trie_options options(std::string(argv[0]) + " trie");
      
      if(options.configure(argc, argv))
        return 1;
      
      ret = main_trie(options);
    }
    else if(strcmp("mhs", argv[1]) == 0) { // MHS mode
      t_mhs_options options(std::string(argv[0]) + " mhs");
      
      if(options.configure(argc, argv))
        return 1;
      
      ret = main_mhs(options);
    }
    else if(strcmp("sandbox", argv[1]) == 0) { // sandbox mode
      ret = main_sandbox();
    }
    else {
      print_help(*argv);
      return 1;
    }
  }
  return ret;

}
