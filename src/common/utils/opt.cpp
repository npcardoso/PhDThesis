#include "opt.h"

#include "types.h"
#include <cstring>
#include <boost/foreach.hpp>

t_opt::t_opt (char short_opt,
              const char * long_opt,
              bool arg,
              bool required,
              std::string help_text) {
    this->short_opt = short_opt;
    this->long_opt = long_opt;
    this->arg = arg;
    this->required = required;
    this->help_text = help_text;
}

std::string t_options::short_opts () const {
    std::stringstream tmp;

    t_opts::const_iterator it = opts.begin();


    while (it != opts.end()) {
        if (it->short_opt) {
            tmp << it->short_opt;

            if (it->arg)
                tmp << ":";
        }

        it++;
    }

    return tmp.str();
}

struct option * t_options::long_opts (int * long_ptr) const {
    struct option * tmp = new struct option[opts.size() + 1];

    t_id i = 0;
    t_opts::const_iterator it = opts.begin();


    while (it != opts.end()) {
        if (it->long_opt) {
            tmp[i].name = it->long_opt;
            tmp[i].has_arg = it->arg;
            tmp[i].flag = 0;
            tmp[i].val = it->short_opt;
            i++;
        }

        it++;
    }

    memset(tmp + i, 0, sizeof(struct option));
    return tmp;
}

t_options::t_options (std::string app_name,
                      bool enable_input,
                      bool enable_output) {
    max_long_opt_size = 0;

    if (enable_input)
        add(t_opt('i', "input", true, false, "Defines input file"));

    if (enable_output)
        add(t_opt('o', "output", true, false, "Defines output file"));

    add(t_opt('h', "help", false, false, "Shows help text"));
    add(t_opt('v', "verbose", false, false, "Enables verbose output"));

    this->app_name = app_name;

    input_ptr = &std::cin;
    input_str = "";

    output_ptr = &std::cout;
    output_str = "";

    devnull.open("/dev/null");
    verbose = &devnull;
}

bool t_options::configure (int argc, char ** argv) {
    std::string short_opts = this->short_opts();

    int long_option = 0;
    struct option * long_opts = this->long_opts(&long_option);

    int c;
    int option_index = 0;
    bool ok = true;


    while ((c = getopt_long(argc, argv, short_opts.c_str(),
                            long_opts, &option_index)) != -1) {
        if (c == '?')
            ok = false;
        else if (c)
            ok = ok && short_opt(c, optarg);
        else
            ok = ok && long_opt(argv[option_index], optarg);
    }


    delete[] long_opts;

    if (argc != optind) {
        std::cerr << "Extraneous arguments: " << "'" << argv[optind++] << "'";

        while (optind != argc)
            std::cerr << ", '" << argv[optind++] << "'";

        std::cerr << std::endl;
        return false;
    }

    return ok;
}

void t_options::show_help () const {
    std::cerr << "Usage: " << app_name << " [options]\n";


    BOOST_FOREACH(auto & o, opts) {
        if (o.short_opt)
            std::cerr << "  " << "-" << o.short_opt;

        if (o.short_opt && o.long_opt)
            std::cerr << ", ";

        if (o.long_opt)
            std::cerr << "--" << o.long_opt;

        for (t_count i = o.long_opt ? strlen(o.long_opt) : 0;
             i < max_long_opt_size; i++)
            std::cerr << " ";

        std::cerr << "\t" << o.help_text << std::endl;
    }
}

bool t_options::short_opt (int c, char * param) {
    switch (c) {
    case 'i':
        finput.open(param);
        input_ptr = &finput;
        input_str = param;
        break;

    case 'o':
        foutput.open(param);
        output_ptr = &foutput;
        output_str = param;
        break;

    case 'v':
        verbose = &std::cerr;
        break;

    case 'h':
        show_help();
        return false;

    default:
        return false;
    }

    return true;
}

bool t_options::long_opt (char * opt, char * param) {
    return false;
}

std::ostream & t_options::print (std::ostream & out) const {
    out << "Input: ";

    if (input_str.size())
        out << '"' << input_str << '"';
    else
        out << "stdin";

    out << ", Output: ";

    if (output_str.size())
        out << '"' << output_str << '"';
    else
        out << "stdout";

    return out;
}

void t_options::add (const t_opt & option) {
    if (option.long_opt) {
        t_count size = strlen(option.long_opt);

        if (size > max_long_opt_size)
            max_long_opt_size = size;
    }

    opts.push_back(option);
}

std::ostream & operator << (std::ostream & out, const t_options & opts) {
    out << '(';
    opts.print(out);
    out << ')';
    return out;
}