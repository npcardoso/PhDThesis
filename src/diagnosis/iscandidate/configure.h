#ifndef __CONFIGURE_iscandidate_H__
#define __CONFIGURE_iscandidate_H__

#include <libdiag/utils/opt.h>

class t_iscandidate_options : public t_options {
public:
    std::ifstream fspectra_input;
    std::istream * spectra_input_ptr;
    std::string spectra_input_str;

    t_iscandidate_options (std::string app_name);

    bool short_opt (int c, char * param);

    virtual std::ostream & print (std::ostream & out) const;

    inline std::istream & spectra_input () const {
        return *spectra_input_ptr;
    }
};

#endif