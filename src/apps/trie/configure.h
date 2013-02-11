#ifndef __CONFIGURE_TRIE_H__
#define __CONFIGURE_TRIE_H__

#include "../../common/opt.h"

class t_trie_options: public t_options {
public:
  std::ifstream ftrie_input;
  std::istream * trie_input_ptr;
  std::string trie_input_str;
  
  t_trie_options(std::string app_name);

  bool short_opt(int c, char * param);
  
  virtual std::ostream & print(std::ostream & out) const;
  
  inline std::istream & trie_input() const {
    return *trie_input_ptr;
  }
};
#endif
