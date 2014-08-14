#include "echo.h"

#include "../../debug.h"

void t_echo_service::operator () (std::istream & in, std::ostream & out) {
    while (in) {
        std::string tmp;
        in >> tmp;
        out << tmp << std::endl;
    }

    debug("Ended Connection");
}