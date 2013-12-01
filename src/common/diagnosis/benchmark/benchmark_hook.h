#ifndef __BENCHMARK_HOOK_H__0734b6b340e40970218b6c20df0194edd574e1c1__
#define __BENCHMARK_HOOK_H__0734b6b340e40970218b6c20df0194edd574e1c1__

#include "diagnosis/diagnosis_system.h"
#include "diagnosis/randomizers/randomizer.h"
#include "diagnosis/structs/spectra.h"
#include "utils/time.h"


namespace diagnosis {
namespace benchmark {
class t_benchmark_hook {
public:
    DEFINE_BOOST_SHARED_PTRS(t_benchmark_hook);

    virtual void init_system (const randomizers::t_system & system) = 0;

    virtual void init (const structs::t_spectra & spectra,
                       const structs::t_candidate & correct) = 0;
    virtual void cleanup () = 0;

    virtual void pre_gen (t_id generator_id,
                          const std::string & name) = 0;
    virtual void post_gen (t_candidate_generator::t_ret_type & D,
                           t_time_interval duration) = 0;

    virtual void pre_rank (t_id ranker_id,
                           const std::string & name) = 0;
    virtual void post_rank (const t_candidate_ranker::t_ret_type & probs,
                            t_time_interval duration) = 0;
};

class t_basic_benchmark_hook : public t_benchmark_hook {
public:
    t_basic_benchmark_hook ();

    virtual void init_system (const randomizers::t_system & system);

    virtual void init (const structs::t_spectra & spectra,
                       const structs::t_candidate & correct);
    virtual void cleanup ();

    virtual void pre_gen (t_id generator_id,
                          const std::string & name);
    virtual void post_gen (t_candidate_generator::t_ret_type & D,
                           t_time_interval duration);

    virtual void pre_rank (t_id ranker_id,
                           const std::string & name);
    virtual void post_rank (const t_candidate_ranker::t_ret_type & probs,
                            t_time_interval duration);

protected:

    inline virtual void _init_system (const randomizers::t_system & system) {}

    inline virtual void _init (const structs::t_spectra & spectra,
                               const structs::t_candidate & correct) {}

    inline virtual void _cleanup () {}

    inline virtual void _pre_gen () {}
    inline virtual void _post_gen (t_candidate_generator::t_ret_type & D,
                                   t_time_interval duration) {}

    inline virtual void _pre_rank () {}
    inline virtual void _post_rank (const t_candidate_ranker::t_ret_type & probs,
                                    t_time_interval duration) {}

    t_id get_generator_id () const;
    t_id get_ranker_id () const;

    const std::string & get_generator_name () const;
    const std::string & get_ranker_name () const;

private:
    t_id generator_id;
    std::string generator_name;
    t_id ranker_id;
    std::string ranker_name;
};
}
}

#endif