#include "configure.h"
#include "diagnosis/structs/trie.h"
#include "types.h"


using namespace diagnosis;

using namespace diagnosis::structs;

int main(int argc, char ** argv){
  t_trie trie;
  t_trie_options options(argv[0]);
      
  if(options.configure(argc, argv))
    return 1;
      
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

