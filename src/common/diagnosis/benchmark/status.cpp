#include "status.h"

#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>

using namespace boost;
using namespace boost::assign;

namespace diagnosis {
namespace benchmark {
t_status_system_init::t_status_system_init (t_id system_id) {
    this->system_id = system_id;
}

t_id t_status_system_init::get_system_id () const {
    return system_id;
}

void t_status_system_init::prepare_entry (t_entry & entry) const {
    entry[STATUS_KEY_SYSTEM] = lexical_cast<std::string> (system_id);
}

t_status_iteration_init::t_status_iteration_init (const t_status_system_init & status,
                                                  t_id iteration_id,
                                                  const structs::t_spectra & spectra,
                                                  const structs::t_candidate & correct) : t_status_system_init(status) {
    this->iteration_id = iteration_id;
    this->spectra = &spectra;
    this->correct = &correct;
}

t_id t_status_iteration_init::get_iteration_id () const {
    return iteration_id;
}

const structs::t_spectra & t_status_iteration_init::get_spectra () const {
    return *spectra;
}

const structs::t_candidate & t_status_iteration_init::get_correct () const {
    return *correct;
}

void t_status_iteration_init::prepare_entry (t_entry & entry) const {
    t_status_system_init::prepare_entry(entry);


    entry[STATUS_KEY_ITERATION] = lexical_cast<std::string> (iteration_id);
}

t_status_post_gen::t_status_post_gen (const t_status_iteration_init & status,
                                      std::string name,
                                      t_time_interval duration,
                                      const t_candidate_generator::t_ret_type & candidates) : t_status_iteration_init(status) {
    this->name = name;
    this->duration = duration;
    this->candidates = &candidates;
}

const std::string & t_status_post_gen::get_gen_name () const {
    return name;
}

t_time_interval t_status_post_gen::get_gen_duration () const {
    return duration;
}

const t_candidate_generator::t_ret_type & t_status_post_gen::get_candidates () const {
    return *candidates;
}

void t_status_post_gen::prepare_entry (t_entry & entry) const {
    t_status_iteration_init::prepare_entry(entry);


    entry[STATUS_KEY_GENERATOR] = name;
}

t_status_post_rank::t_status_post_rank (const t_status_post_gen & status,
                                        std::string name,
                                        t_time_interval duration,
                                        const t_candidate_ranker::t_ret_type & probs) : t_status_post_gen(status) {
    this->name = name;
    this->duration = duration;
    this->probs = &probs;
}

const std::string & t_status_post_rank::get_rank_name () const {
    return name;
}

t_time_interval t_status_post_rank::get_rank_duration () const {
    return duration;
}

const t_candidate_ranker::t_ret_type & t_status_post_rank::get_probs () const {
    return *probs;
}

void t_status_post_rank::prepare_entry (t_entry & entry) const {
    t_status_post_gen::prepare_entry(entry);


    entry[STATUS_KEY_RANKER] = name;
}
}
}