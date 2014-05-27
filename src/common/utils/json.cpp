#include "json.h"

#include "../types.h"

void json_copy_object (std::istream & in, std::ostream & out) {
    bool in_str = false;
    bool escape = false;
    t_count stack_size = 0;

    char c;


    while (in >> c) {
        if (escape) {
            escape = false;
        }
        else if (c == '"') {
            in_str = !in_str;
        }
        else if (in_str) {
            escape = c == '\\';
        }
        else if (c == '[' || c == '{') {
            stack_size++;
        }
        else if (c == ']' || c == '}') {
            stack_size--;

            if (!stack_size) {
                out << c;
                break;
            }
        }

        if (stack_size && (in_str || isprint(c)))
            out << c;
    }
}