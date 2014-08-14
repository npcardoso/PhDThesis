#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../configuration/configuration.h"

#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

using boost::property_tree::ptree;
using boost::property_tree::ptree_bad_path;
using boost::property_tree::read_json;
using boost::property_tree::write_json;
using namespace std;
using namespace diagnostic;
//using namespace diagnostic::algorithms;
using namespace diagnostic::configuration;


extern "C" {
void run_diagnostic(const char * request, void ** response)
{

  ptree pt;
  stringstream ss (request);

  read_json (ss, pt);

  t_const_ptr<t_diagnostic_system> dj = construct_diagnostic_system (pt);
  cout << *dj;
  *response = strdup("hello world");
}

void cleanup_diagnostic(void * response)
{
	free(response);
}
}
