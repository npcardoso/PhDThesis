#ifndef __COMMON_SERIALIZATION_TOKENIZERS_JSON_H__
#define __COMMON_SERIALIZATION_TOKENIZERS_JSON_H__

#include <boost/spirit/include/lex_lexertl.hpp>

namespace lex = boost::spirit::lex;

template <typename Lexer>
class t_json_tokenizer : public lex::lexer<Lexer> {
public:
  enum token_ids {
    ID_ARRAY_START = 1000,
    ID_ARRAY_END,
    ID_MAP_START,
    ID_MAP_END,
    ID_COMMA,
    ID_COLON,
    ID_STRING,
    ID_NUMBER,
    ID_TRUE,
    ID_FALSE,
    ID_NULL,
    ID_IGNORE
  };

  t_json_tokenizer() {
    this->self.add ("\\\"([^\\\"\\\\]|(\\\\[\\\\\\\"\\/bfnrt])|\\\\u\\d{4})*\\\"", ID_STRING)
      ("-?([1-9][0-9]+|0)(\\.[0-9]+)?([eE][+-]?[0-9]+)?", ID_NUMBER)
      ("true", ID_TRUE)
      ("false", ID_FALSE)
      ("null", ID_NULL)
      ("\\{", ID_MAP_START)
      ("\\}", ID_MAP_END)
      ("\\[", ID_ARRAY_START)
      ("\\]", ID_ARRAY_END)
      (",", ID_COMMA)
      (":", ID_COLON)
      ("\\s+", ID_IGNORE ); // ignore whitespace
      
  }
};

#endif
