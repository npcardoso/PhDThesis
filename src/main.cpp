#include "apps/trie/main.h"
#include "apps/mhs/main.h"

#include "common/types.h"
#include "common/utils.h"


#include <cstring>
#include <cstdio>

#include <iostream>

int main(int argc, char ** argv){
  int ret = 0;


  if(argc == 1) {
    std::cerr << "Usage: " << argv[0] << " <mhs | trie> [options]\n";
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
      t_mhs_options<t_count> options(std::string(argv[0]) + " mhs");
      
      if(options.configure(argc, argv))
        return 1;
      
      ret = main_mhs(options);
    } else {
      std::cerr << "Usage: " << argv[0] << " <mhs | trie> [options]\n";
      return 1;
    }
  }

}
