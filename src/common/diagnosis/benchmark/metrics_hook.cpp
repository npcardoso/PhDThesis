#include "metrics_hook.h"

#include <boost/foreach.hpp>
#include <fstream>

namespace diagnosis {
namespace benchmark {
t_metrics_hook::t_metrics_hook (std::string d) : t_writer_hook(d) {}

t_metrics_hook & t_metrics_hook::operator << (t_metric::t_ptr & metric) {
    metrics_list.push_back(metric);
    return *this;
}

t_metrics_hook & t_metrics_hook::operator << (t_metric * metric) {
    metrics_list.push_back(t_metric::t_ptr(metric));
    return *this;
}

void t_metrics_hook::_init (const structs::t_spectra & spectra,
                            const structs::t_candidate & correct) {
    assert(this->spectra == NULL);
    assert(this->correct == NULL);
    assert(D == NULL);
    this->spectra = &spectra;
    this->correct = &correct;
}

void t_metrics_hook::_cleanup () {
    spectra = NULL;
    correct = NULL;
    D = NULL;
}

void t_metrics_hook::_pre_gen () {}
void t_metrics_hook::_post_gen (t_candidate_generator::t_ret_type & D,
                                t_time_interval duration) {
    assert(spectra != NULL);
    assert(correct != NULL);
    this->D = &D;
}

void t_metrics_hook::_pre_rank () {}
void t_metrics_hook::_post_rank (const t_candidate_ranker::t_ret_type & probs,
                                 t_time_interval duration) {
    assert(spectra != NULL);
    assert(correct != NULL);
    assert(D != NULL);
    structs::t_diagnosis_report dr(*D, probs);

    t_metric::t_arguments args;

    BOOST_FOREACH(t_metrics_list::value_type & metric,
                  metrics_list) {
        std::string ret = (* metric)(* spectra, * correct, * D, probs, dr, args);


        assert(args.count(metric->key()) == 0);
        args[metric->key()] = ret;
    }

    std::ofstream f;

    if (open_file("metrics.csv", f, true))
        f << "randomizer, iteration, generator, ranker, metric_name, value" << std::endl;

    BOOST_FOREACH(t_metric::t_arguments::value_type & metric,
                  args) {
        f << get_randomizers() << ", ";
        f << get_iterations() << ", ";
        f << get_generator_id() << ", ";
        f << get_ranker_id() << ", ";
        f << "\"" << metric.first << "\", ";
        f << metric.second << std::endl;
    }
    f.close();
}
}
}