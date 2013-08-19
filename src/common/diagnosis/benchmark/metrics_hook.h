#ifndef __METRICS_HOOK_H_0b0885703861f344448f13adf9cea1633b73bc5c__
#define __METRICS_HOOK_H_0b0885703861f344448f13adf9cea1633b73bc5c__

#include "diagnosis/benchmark/writer_hook.h"
#include "diagnosis/benchmark/metrics.h"

namespace diagnosis {
namespace benchmark {
class t_metrics_hook : public t_writer_hook {
public:
    t_metrics_hook (std::string d);

    t_metrics_hook & operator << (t_metric::t_ptr & metric);
    t_metrics_hook & operator << (t_metric * metric);

protected:
    virtual void _init (const structs::t_spectra & spectra,
                        const structs::t_candidate & correct);

    virtual void _cleanup ();

    virtual void _pre_gen ();
    virtual void _post_gen (t_candidate_generator::t_ret_type & D,
                            t_time_interval duration);

    virtual void _pre_rank ();
    virtual void _post_rank (const t_candidate_ranker::t_ret_type & probs,
                             t_time_interval duration);


private:
    typedef std::list<t_metric::t_ptr> t_metrics_list;
    t_metrics_list metrics_list;

    const structs::t_spectra * spectra;
    const structs::t_candidate * correct;
    const t_candidate_generator::t_ret_type * D;
};
}
}

#endif