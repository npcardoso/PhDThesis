#include "configuration.h"

#include <boost/foreach.hpp>

using boost::property_tree::ptree;
using boost::property_tree::ptree_bad_path;
using namespace std;

namespace diagnostic {
namespace configuration {



t_const_ptr<t_algorithm::t_configs> parse_configs(const ptree & pt){
    t_ptr<t_algorithm::t_configs> ptr = t_algorithm::new_configs();

    BOOST_FOREACH(const ptree::value_type & e, pt) {
        (*ptr)[e.first] = e.second.get_value<string>();
    }

    return ptr;
}


t_const_ptr<t_algorithm> parse_algorithm (const ptree & pt){
    string name = pt.get<string>("name");

    t_const_ptr<t_algorithm::t_configs> configs;
    try {
        configs = parse_configs(pt.get_child("configs"));
    }
    catch(ptree_bad_path e) {
        configs = t_const_ptr<t_algorithm::t_configs>(new t_algorithm::t_configs());
    }

    return t_const_ptr<t_algorithm>(new t_algorithm(name, configs));
}

t_const_ptr<t_diagnostic_system> construct_diagnostic_system (const ptree & pt,
                                                              const t_algorithm_creator & ac) {
    t_ptr<t_diagnostic_system> dj(new t_diagnostic_system());

    BOOST_FOREACH(const ptree::value_type & e, pt.get_child("generators")) {
        t_const_ptr<t_algorithm> a = parse_algorithm(e.second);
        t_const_ptr<t_candidate_generator> cg;
        if(a)
            cg = ac.generator(*a);
        dj->add_generator(cg);
    }

    BOOST_FOREACH(const ptree::value_type & e, pt.get_child("rankers")) {
        t_const_ptr<t_algorithm> a = parse_algorithm(e.second);
        t_const_ptr<t_candidate_ranker> cr;
        if(a)
            cr = ac.ranker(*a);
        dj->add_ranker(cr);
    }

    BOOST_FOREACH(const ptree::value_type & e, pt.get_child("connections")) {
        int from = e.second.get("from", -1);
        int to = e.second.get("to", -1);
        dj->add_connection(from + 1, to + 1);
    }


    return dj;
}
}
}
