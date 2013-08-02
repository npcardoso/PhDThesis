#include "utils/debug.h"
#include <iostream>
#include <string>
#include "instrumentation/sinks/collector.h"
#include "converters/observation_graphviz.h"
#include "serialization/unserializers/json.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#define BUF_SIZE 102400
using namespace converters;
using namespace instrumentation;
using namespace instrumentation::sinks;

int main () {
    try {
        char tmp[BUF_SIZE];
        std::cin.getline(tmp, BUF_SIZE);


        boost::property_tree::ptree tree;
        std::stringstream stream(tmp);
        try {
            read_json(stream, tree);
        }
        catch (boost::property_tree::json_parser::json_parser_error & e) {
            std::cout << stream.str();
            std::cerr << e.what() << std::endl;
        }

        t_observation_collector::t_ptr collector(new t_observation_collector());
        t_json_observation_unserializer unserializer(collector);
        unserializer(tree);

        t_observations_to_graphviz converter;
        converter.start(std::cout);
        BOOST_FOREACH(t_transaction_observation::t_ptr tr,
                      collector->transactions)
        converter(*tr, std::cout);
        converter.end(std::cout);
    }
    catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
        throw e;
    }

    return 0;
}