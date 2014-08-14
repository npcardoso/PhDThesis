#include "status.h"

#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>

using namespace boost;
using namespace boost::assign;

namespace diagnostic {
namespace benchmark {
t_status_iteration_init::t_status_iteration_init (t_id iteration_id,
                                                  t_time_interval start,
                                                  const t_const_ptr<structs::t_spectra> & spectra,
                                                  const t_const_ptr<structs::t_candidate> & correct) {
    assert(spectra.get() != NULL);
    assert(correct.get() != NULL);

    this->iteration_id = iteration_id;
    this->start = start;
    this->spectra = spectra;
    this->correct = correct;
}

t_id t_status_iteration_init::get_iteration_id () const {
    return iteration_id;
}

t_time_interval t_status_iteration_init::get_iteration_start () const {
    return start;
}

const structs::t_spectra & t_status_iteration_init::get_spectra () const {
    return *spectra;
}

const structs::t_candidate & t_status_iteration_init::get_correct () const {
    return *correct;
}

void t_status_iteration_init::prepare_entry (t_entry & entry) const {
    entry[STATUS_KEY_ITERATION] = lexical_cast<std::string> (iteration_id);
}

t_status_post_gen::t_status_post_gen (const t_status_iteration_init & status,
                                      std::string name,
                                      t_time_interval start,
                                      t_time_interval end,
                                      const t_const_ptr<t_candidate_generator::t_ret_type> & candidates) : t_status_iteration_init(status) {
    assert(candidates.get() != NULL);
    this->name = name;
    this->start = start;
    this->end = end;
    this->candidates = candidates;
}

const std::string & t_status_post_gen::get_gen_name () const {
    return name;
}

t_time_interval t_status_post_gen::get_gen_duration () const {
    return end - start;
}

t_time_interval t_status_post_gen::get_gen_start () const {
    return start;
}

t_time_interval t_status_post_gen::get_gen_end () const {
    return end;
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
                                        t_time_interval start,
                                        t_time_interval end,
                                        const t_const_ptr<t_candidate_ranker::t_ret_type> & probs) : t_status_post_gen(status) {
    assert(probs.get() != NULL);
    this->name = name;
    this->start = start;
    this->end = end;
    this->probs = probs;
}

const std::string & t_status_post_rank::get_rank_name () const {
    return name;
}

t_time_interval t_status_post_rank::get_rank_duration () const {
    return end - start;
}

t_time_interval t_status_post_rank::get_rank_start () const {
    return start;
}

t_time_interval t_status_post_rank::get_rank_end () const {
    return end;
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
