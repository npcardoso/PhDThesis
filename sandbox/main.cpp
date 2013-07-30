#include "utils/debug.h"
#include <iostream>
#include <string>
#include "converters/observation_spectra.h"
#include "diagnosis/algorithms/mhs.h"
#include "diagnosis/heuristics/similarity.h"
#include "diagnosis/heuristics/sort.h"
#include "instrumentation/sinks/collector.h"
#include "server/servers/tcp_server.h"
#include "server/services/threaded.h"
#include "server/services/json.h"
#include "serialization/unserializers/json.h"
#include "utils/json.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#define BUF_SIZE 10240


using namespace converters;
using namespace diagnosis;
using namespace diagnosis::algorithms;
using namespace diagnosis::structs;
using namespace instrumentation;
using namespace instrumentation::sinks;


class t_mhs_service : public t_service {
public:
    virtual void operator () (std::istream & in,
                              std::ostream & out) {
        t_count_spectra res;


        while (in) {
            std::stringstream tmp;
            json_copy_object(in, tmp);

            if (!tmp.str().size())
                continue;

            boost::property_tree::ptree tree;
            try {
                read_json(tmp, tree);
            }
            catch (boost::property_tree::json_parser::json_parser_error & e) {
                std::cout << tmp.str();
                std::cerr << e.what() << std::endl;
            }

            t_observation_collector::t_ptr collector(new t_observation_collector());
            t_json_observation_unserializer unserializer(collector);
            unserializer(tree);


            t_observations_to_count_spectra converter;
            BOOST_FOREACH(t_transaction_observation::t_ptr tr,
                          collector->transactions) {
                tr->flatten();
                converter(*tr, res);
            }
        }

        std::cout << res << std::endl;

        t_trie D;
        t_heuristic heuristic;
        heuristic.push(new heuristics::t_ochiai());
        heuristic.push(new heuristics::t_sort());
        t_mhs mhs(heuristic);

        mhs.calculate(res, D);
        std::cout << D << std::endl;
    }
};

int main (int argc, char ** argv) {
    int src_port = 12345;


    try {
        boost::asio::io_service io_service;

        t_service::t_ptr service(new t_mhs_service());

        tcp_server(io_service, src_port, *service);
        io_service.run();
    }
    catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}