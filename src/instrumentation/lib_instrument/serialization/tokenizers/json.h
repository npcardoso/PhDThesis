#ifndef __COMMON_SERIALIZATION_TOKENIZERS_JSON_H__
#define __COMMON_SERIALIZATION_TOKENIZERS_JSON_H__

#include <boost/spirit/include/lex_lexertl.hpp>

namespace lex = boost::spirit::lex;

typedef lex::lexertl::token<char const *, boost::mpl::vector0<>, boost::mpl::false_> t_json_default_token_type;
typedef lex::lexertl::actor_lexer<t_json_default_token_type> t_json_default_lexer_type;

template <typename Lexer = t_json_default_lexer_type>
class t_json_tokenizer : public lex::lexer<Lexer> {
public:
    typedef typename Lexer::iterator_type t_iterator;
    enum t_token_ids {
        ID_IGNORE = 0,
        ID_ARRAY_START = 1,
        ID_ARRAY_END = 1 << 1,
            ID_MAP_START = 1 << 2,
            ID_MAP_END = 1 << 3,
            ID_COMMA = 1 << 4,
            ID_COLON = 1 << 5,
            ID_STRING = 1 << 6,
            ID_NUMBER = 1 << 7,
            ID_TRUE = 1 << 8,
            ID_FALSE = 1 << 9,
            ID_NULL = 1 << 10
    };

    t_json_tokenizer () {
        this->self.add("\\\"([^\\\"\\\\]|(\\\\[\\\\\\\"\\/bfnrt])|\\\\u\\d{4})*\\\"", ID_STRING)
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
            ("\\s+", ID_IGNORE); // ignore whitespace
    }
};

#endif