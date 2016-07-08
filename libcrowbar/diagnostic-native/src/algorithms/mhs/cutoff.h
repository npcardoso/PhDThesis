#ifndef __CUTOFF_H_eef6fe3d54a024fdbc0a8263ef12470accf5f2b0__
#define __CUTOFF_H_eef6fe3d54a024fdbc0a8263ef12470accf5f2b0__

#include "../similarity.h"
#include "../../utils/time.h"
#include "../../structs/trie.h"

namespace diagnostic {
namespace algorithms {
class t_basic_cutoff {
public:
    typedef t_algorithm::t_configs t_configs;

    virtual bool stop (t_count depth) const;
    virtual bool stop (const t_single_component_ranking & rank,
                       t_id pos,
                       const t_trie & D,
                       t_time_interval time_elapsed) const;

    virtual void json_configs (t_configs & out) const = 0;


protected:
    inline virtual ~t_basic_cutoff () {}
};

class t_no_cutoff : public t_basic_cutoff  {
public:
    inline virtual void json_configs (t_configs & out) const {}
};

class t_cutoff : public t_basic_cutoff  {
public:
    t_cutoff ();
    virtual bool stop (t_count depth) const;

    virtual bool stop (const t_single_component_ranking & rank,
                       t_id pos,
                       const t_trie & D,
                       t_time_interval time_elapsed) const;

    virtual void json_configs (t_configs & out) const;

public:
    t_count max_depth, max_candidates;
    t_time_interval max_time;
    float lambda;
    t_score min_score;
};
}
}
#endif