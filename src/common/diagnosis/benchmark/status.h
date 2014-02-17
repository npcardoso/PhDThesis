#ifndef __STATUS_H_002ec00416d0bb9fe059ff731dccb22e60e633a2__
#define __STATUS_H_002ec00416d0bb9fe059ff731dccb22e60e633a2__

#include "diagnosis/benchmark/types.h"
#include "diagnosis/diagnosis_system.h"
#include "utils/time.h"


#define STATUS_KEY_ITERATION "iteration"
#define STATUS_KEY_GENERATOR "generator"
#define STATUS_KEY_RANKER "ranker"

namespace diagnosis {
namespace benchmark {
class t_status {
public:
    virtual void prepare_entry (t_entry & entry) const = 0;
    inline ~t_status () {}
protected:
    inline t_status () {}
};

class t_status_iteration_init : public t_status {
public:
    t_status_iteration_init (t_id iteration_id,
                             t_time_interval start,
                             const t_const_ptr<structs::t_spectra> & spectra,
                             const t_const_ptr<structs::t_candidate> & correct);

    t_id get_iteration_id () const;

    t_time_interval get_iteration_start () const;

    const structs::t_spectra & get_spectra () const;
    const structs::t_candidate & get_correct () const;

    virtual void prepare_entry (t_entry & entry) const;

private:
    t_id iteration_id;
    t_time_interval start;
    t_const_ptr<structs::t_spectra> spectra;
    t_const_ptr<structs::t_candidate> correct;
};

class t_status_post_gen : public t_status_iteration_init {
public:
    t_status_post_gen (const t_status_iteration_init & status,
                       std::string name,
                       t_time_interval start,
                       t_time_interval end,
                       const t_const_ptr<t_candidate_generator::t_ret_type> & candidates);

    virtual const std::string & get_gen_name () const;

    virtual t_time_interval get_gen_duration () const;
    virtual t_time_interval get_gen_start () const;
    virtual t_time_interval get_gen_end () const;

    virtual const t_candidate_generator::t_ret_type & get_candidates () const;

    virtual void prepare_entry (t_entry & entry) const;

private:
    std::string name;
    t_time_interval start;
    t_time_interval end;

    t_const_ptr<t_candidate_generator::t_ret_type> candidates;
};

class t_status_post_rank : public t_status_post_gen {
public:
    t_status_post_rank (const t_status_post_gen & status,
                        std::string name,
                        t_time_interval start,
                        t_time_interval end,
                        const t_const_ptr<t_candidate_ranker::t_ret_type> & probs);

    virtual const std::string & get_rank_name () const;

    virtual t_time_interval get_rank_duration () const;
    virtual t_time_interval get_rank_start () const;
    virtual t_time_interval get_rank_end () const;

    virtual const t_candidate_ranker::t_ret_type & get_probs () const;

    virtual void prepare_entry (t_entry & entry) const;

private:
    std::string name;
    t_time_interval start;
    t_time_interval end;

    t_const_ptr<t_candidate_ranker::t_ret_type> probs;
};
}
}
#endif