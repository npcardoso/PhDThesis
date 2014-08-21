#include "main.h"

#include "../structs/count_spectrum.h"
#include "../configuration/configuration.h"
#include "../diagnostic_report.h"
#include "../serialization/json.h"
#include "../runner.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
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
using namespace diagnostic::configuration;

#define MSG_TYPE_DIAGNOSTIC "diagnostic"



std::ostream & json_write_diagnostic_response (std::ostream & out,
                                               const t_diagnostic_system & ds,
                                               const t_diagnostic_report & dr) {
    out << '{';
    json_write(out, "type") << ':';
    json_write(out, MSG_TYPE_DIAGNOSTIC) << ',';
    json_write(out, "system") << ':';
    json_write(out, ds) << ',';
    json_write(out, "report") << ':';
    json_write(out, dr) << '}';
    return out;
}

void run_diagnostic (const ptree & pt,
                     char ** response) {
    stringstream ss;

    t_const_ptr<t_diagnostic_system> ds = construct_diagnostic_system (pt.get_child("system"));

    t_count_spectrum spectrum;
    ss << pt.get<std::string>("spectrum");
    ss >> spectrum;

    t_basic_runner runner;

    auto dr = runner.run(spectrum, ds);

    ss.str("");
    json_write_diagnostic_response(ss, *ds, *dr);

    *response = strdup(ss.str().c_str());
}


void run (const char * request,
          char ** response) {

    ptree pt;
    stringstream ss (request);

    read_json (ss, pt);

    string request_type = pt.get<string>("type", string());

    if(request_type == MSG_TYPE_DIAGNOSTIC)
        run_diagnostic(pt, response);



    std::cout << " -----Request----- " << std::endl;
    std::cout << request << std::endl;
    std::cout << " -----Request----- " << std::endl;
    std::cout << " -----Response----- " << std::endl;
    std::cout << *response << std::endl;
    std::cout << " -----Response----- " << std::endl;
}

void cleanup(char * response) {
    free(response);
}
