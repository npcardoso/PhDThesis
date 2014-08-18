#include "main.h"

#include "../configuration/configuration.h"
#include "../diagnostic_report.h"

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


void run_diagnostic(const char * request, char ** response) {

    ptree pt;
    stringstream ss (request);
    cout << request << std::endl;

    read_json (ss, pt);

    t_const_ptr<t_diagnostic_system> dj = construct_diagnostic_system (pt);
    cout << *dj << std::endl;


    t_diagnostic_report dr(dj);
    t_trie * D;

    D = new t_trie();
    D->add(t_candidate(1,2,3,45,0));
    D->add(t_candidate(2,3,4,0));
    D->add(t_candidate(8,3,4,0));
    dr.add(1, t_const_ptr<t_trie> (D));


    D = new t_trie();
    D->add(t_candidate(1,9,3,4,0));
    D->add(t_candidate(4,3,5,0));
    D->add(t_candidate(8,7,4,0));
    dr.add(2, t_const_ptr<t_trie> (D));

    t_ptr<t_candidate_ranker::t_ret_type> scores(new t_candidate_ranker::t_ret_type()) ;
    scores->push_back(0.25);
    scores->push_back(0.25);
    scores->push_back(0.5);

    dr.add(t_connection(1,1), scores);
    dr.add(t_connection(1,2), scores);
    dr.add(t_connection(2,2), scores);


    cout << dr << std::endl << "-------------" << std::endl;

    *response = strdup("hello world");
}

void cleanup_diagnostic(char * response)
{
    free(response);
}
