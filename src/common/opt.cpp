#include "opt.h"

#include "types.h"

t_opt::t_opt(char short_opt, 
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

std::string t_options::short_opts() const {
  std::stringstream tmp;

  typename t_opts::const_iterator it = opts.begin();
  while(it != opts.end()) {
    if(it->short_opt){
      tmp << it->short_opt;
      if(it->arg)
        tmp << ":";
    }
    it++;
  }
  return tmp.str();
}

struct option * t_options::long_opts(int * long_ptr) const {
  struct option * tmp = new struct option[opts.size() + 1];
  
  t_id i = 0;
  typename t_opts::const_iterator it = opts.begin();

  while(it != opts.end()) {
    if(it->long_opt)
      tmp[i++] = {it->long_opt, it->arg, 0, it->short_opt};
    it++;
  }
  
  tmp[i] = {0,0,0,0};
  return tmp;
}

t_options::t_options(std::string app_name,
                     bool enable_input, 
                     bool enable_output) {

  if(enable_input)
    add(t_opt('i', "input", true, false, "Defines input file"));

  if(enable_output)
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

bool t_options::configure(int argc, char ** argv) {
  std::string short_opts = this->short_opts();

  int long_option = 0;
  struct option * long_opts = this->long_opts(&long_option);

  int c;
  int option_index = 0;
  bool error = false;

  while ((c = getopt_long(argc, argv, short_opts.c_str(),
                          long_opts, &option_index)) != -1) {
    if(c)
      error |= short_opt(c, optarg);
    else
      error |= long_opt(argv[option_index], optarg);
  }
  delete[]long_opts;
  return error;
}
  
void t_options::show_help() const {
  std::cerr << "Usage: " << app_name << " [options]\n";
  
  typename t_opts::const_iterator it = opts.begin();
  while(it != opts.end()) {
    if(it->short_opt)
      std::cerr << "\t" << "-" << it->short_opt;
    if(it->short_opt && it->long_opt)
      std::cerr << ",";
    if(it->long_opt)
      std::cerr << "--" << it->long_opt;
    std::cerr << "\t" << it->help_text << std::endl;
    it++;
  }
}

bool t_options::short_opt(int c, char * param) {
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
    return true;
  default:
    return true;
  }
  return false;
}

bool t_options::long_opt(char * opt, char * param) {
  return true;
}

std::ostream & t_options::print(std::ostream & out) const {
  out << "Input: ";
  if(input_str.size())
    out << '"' << input_str << '"';
  else
    out << "stdin";
  out << ", Output: ";
  if(output_str.size())
    out << '"' << output_str << '"';
  else
    out << "stdout";
  return out;
}

std::ostream & operator <<(std::ostream & out, const t_options & opts) {
  out << '(';
  opts.print(out);
  out << ')';
  return out;
}
