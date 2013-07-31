#include "configure.h"

t_trie_options::t_trie_options (std::string app_name) : t_options(app_name, true, true) {
    trie_input_ptr = &std::cin;
    trie_input_str = "";

    add(t_opt('t', "trie", true, false, "Defines candidate file"));
}

bool t_trie_options::short_opt (int c, char * param) {
    switch (c) {
    case 't':
        ftrie_input.open(param);
        trie_input_ptr = &ftrie_input;
        trie_input_str = param;
        break;

    default:
        return t_options::short_opt(c, param);
    }

    return false;
}

std::ostream & t_trie_options::print (std::ostream & out) const {
    t_options::print(out);


    out << ", Trie: ";

    if (trie_input_str.size())
        out << '"' << trie_input_str << '"';
    else
        out << "stdin";

    return out;
}