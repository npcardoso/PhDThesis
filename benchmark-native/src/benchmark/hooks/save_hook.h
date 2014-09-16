#ifndef __SAVE_HOOK_H__9bfc7dc852528cb46b831eae7a7f4e308bf88de4__
#define __SAVE_HOOK_H__9bfc7dc852528cb46b831eae7a7f4e308bf88de4__

#include "hook.h"

namespace diagnostic {
namespace benchmark {
class t_save_hook : public t_hook {
public:
    virtual void trigger_event (t_collector & collector,
                                const t_status_iteration_init & status) const;

    virtual void trigger_event (t_collector & collector,
                                const t_status_post_gen & status) const;

    virtual void trigger_event (t_collector & collector,
                                const t_status_post_rank & status) const;
};
}
}
#endif