#include "main.h"
#include "../../structs/trie.h"
#include "../../types.h"


int main_trie(const t_trie_options & options) {
  t_trie trie;
    
  options.trie_input() >> trie;
  
  while(true){
    t_candidate c;
    options.input() >> c;
    if(!c.size())
      break;
    options.output() << trie.is_composite(c) << std::endl;
  }
  return 0;
}

