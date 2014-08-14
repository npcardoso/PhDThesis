#include <crowbar/diagnostic/diag.h>

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
using namespace diagnostic::algorithms;
using namespace diagnostic::configuration;


void print(ptree pt, string prefix=""){
    BOOST_FOREACH(ptree::value_type & e, pt) {
        cout << prefix +"> " + e.first << ":"  << e.second.get_value<string>() << std::endl;
        print(e.second, prefix+" ");
    }
}







int main() {
  ptree pt;
  read_json (std::cin, pt);
  print(pt);
  cout << std::endl;


  t_const_ptr<t_diagnostic_system> dj = construct_diagnostic_system (pt);
  cout << *dj;
  return 0;
}
