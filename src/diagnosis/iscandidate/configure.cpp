#include "configure.h"

t_iscandidate_options::t_iscandidate_options (std::string app_name) : t_options(app_name, true, true) {
    spectra_input_ptr = &std::cin;
    spectra_input_str = "";

    add(t_opt('s', "spectra", true, false, "Defines spectra file"));
}

bool t_iscandidate_options::short_opt (int c, char * param) {
    switch (c) {
    case 's':
        fspectra_input.open(param);
        spectra_input_ptr = &fspectra_input;
        spectra_input_str = param;
        break;

    default:
        return t_options::short_opt(c, param);
    }

    return false;
}

std::ostream & t_iscandidate_options::print (std::ostream & out) const {
    t_options::print(out);


    out << ", iscandidate: ";

    if (spectra_input_str.size())
        out << '"' << spectra_input_str << '"';
    else
        out << "stdin";

    return out;
}