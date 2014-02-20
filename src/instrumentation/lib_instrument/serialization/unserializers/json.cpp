#include "json.h"
#include "instrumentation/sinks/transaction_factory.h"
#include "utils/base64.h"
#include <boost/foreach.hpp>


using boost::property_tree::ptree;
using namespace instrumentation;
using namespace instrumentation::sinks;

t_json_observation_unserializer::t_json_observation_unserializer (t_ptr<t_observation_sink> sink) : sink(sink) {}

void t_json_observation_unserializer::operator () (const boost::property_tree::ptree & tree,
                                                   bool skip_errors) {
    t_ptr<t_observation_sink> transaction_factory(new t_transaction_factory(sink));


    BOOST_FOREACH(const auto & v, tree)
    read_transaction(v.second, transaction_factory);
}

void t_json_observation_unserializer::read_probe (const boost::property_tree::ptree & tree,
                                                  t_ptr<t_observation_sink> sink) {
    t_ptr<t_probe_observation> probe(new t_probe_observation());


    probe->c_id = tree.get<t_construct_id> ("cid");
    probe->time = tree.get<t_time_interval> ("t", -1);

    boost::optional<const ptree &> state = tree.get_child_optional("s");

    if (state) {
        BOOST_FOREACH(const ptree::value_type & v, *state) {
            std::string str = base64_decode(v.second.data());


            probe->read_variable(str.c_str(), str.size());
        }
    }

    (*sink) << probe;
}

void t_json_observation_unserializer::read_oracle (const boost::property_tree::ptree & tree,
                                                   t_ptr<t_observation_sink> sink) {
    t_ptr<t_oracle_observation> oracle(new t_oracle_observation());


    oracle->c_id = tree.get<t_construct_id> ("cid");
    oracle->time = tree.get<t_time_interval> ("t", -1);
    oracle->health = tree.get<float> ("h");
    oracle->confidence = tree.get<float> ("c", 1);
    (*sink) << oracle;
}

void t_json_observation_unserializer::read_transaction (const boost::property_tree::ptree & tree,
                                                        t_ptr<t_observation_sink> sink) {
    const ptree & tmp_tree = tree.get_child("cid");


    if (tmp_tree.size() != 2) {
        return;
    }

    t_construct_id start = tmp_tree.begin()->second.get_value<t_construct_id> (),
                   end = (++tmp_tree.begin())->second.get_value<t_construct_id> ();

    t_time_interval t_start = -1, t_end = -1;

    if (boost::optional<const ptree &> times = tree.get_child_optional("t")) {
        if (times->size() == 2) {
            t_start = times->begin()->second.get_value<t_time_interval> ();
            t_end = (++times->begin())->second.get_value<t_time_interval> ();
        }
    }


    (*sink) << t_ptr<t_transaction_observation> (new t_transaction_observation(t_start, start));

    if (boost::optional<const ptree &> sub_transactions = tree.get_child_optional("tr")) {
        BOOST_FOREACH(const ptree::value_type & v, *sub_transactions) {
            read_transaction(v.second, sink);
        }
    }

    if (boost::optional<const ptree &> oracles = tree.get_child_optional("o")) {
        BOOST_FOREACH(const ptree::value_type & v, *oracles) {
            read_oracle(v.second, sink);
        }
    }

    if (boost::optional<const ptree &> probes = tree.get_child_optional("p")) {
        BOOST_FOREACH(const ptree::value_type & v, *probes) {
            read_probe(v.second, sink);
        }
    }

    (*sink) << t_ptr<t_transaction_observation> (new t_transaction_observation(0, 0, t_end, end));
}