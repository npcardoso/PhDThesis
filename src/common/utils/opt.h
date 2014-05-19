#ifndef __OPT_H_a99fd11755d4e31b0eb9227b503a680feb56b392__
#define __OPT_H_a99fd11755d4e31b0eb9227b503a680feb56b392__

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <getopt.h>

#include "types.h"

template <class TYPE>
bool verb_strtof (char * buf, TYPE & dest, bool non_negative=false) {
    char * end_ptr;
    double tmp;


    tmp = strtod(buf, &end_ptr);

    if (end_ptr == buf || *end_ptr || (non_negative && tmp < 0)) {
        std::cerr << "Invalid " << (non_negative ? "non-negative " : "") << "float number: '" << buf << "'" << std::endl;
        return false;
    }

    dest = tmp;
    return true;
}

template <class TYPE>
bool verb_strtoi (char * buf, TYPE & dest, bool non_negative=false) {
    char * end_ptr;
    long tmp;


    tmp = strtol(buf, &end_ptr, 10);

    if (end_ptr == buf || *end_ptr || (non_negative && tmp < 0)) {
        std::cerr << "Invalid " << (non_negative ? "non-negative " : "") << "integer number: '" << buf << "'" << std::endl;
        return false;
    }

    dest = tmp;
    return true;
}

class t_opt {
public:
    char short_opt;
    const char * long_opt;
    bool arg;
    bool required;
    std::string help_text;

    t_opt (char short_opt,
           const char * long_opt,
           bool arg,
           bool required,
           std::string help_text);
};

class t_options {
public:
    std::ostream * verbose;

    std::string app_name;

    std::ifstream finput;
    std::istream * input_ptr;
    std::string input_str;

    std::ofstream foutput;
    std::ostream * output_ptr;
    std::string output_str;

    t_options (std::string app_name,
               bool enable_input=false,
               bool enable_output=false);

    bool configure (int argc, char ** argv);

    void show_help () const;

    virtual bool short_opt (int c, char * param);
    virtual bool long_opt (char * opt, char * param);

    inline std::istream & input () const {
        return *input_ptr;
    }

    inline std::ostream & output () const {
        return *output_ptr;
    }

    inline std::ostream & debug () const {
        return *verbose;
    }

    virtual std::ostream & print (std::ostream & out) const;
    std::ofstream devnull;

protected:
    void add (const t_opt & option);

    std::string short_opts () const;
    struct option * long_opts (int * long_ptr) const;

private:
    typedef std::vector<t_opt> t_opts;

    t_opts opts;
    t_count max_long_opt_size;
};

std::ostream & operator << (std::ostream & out, const t_options & opts);

#endif