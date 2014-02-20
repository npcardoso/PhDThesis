#ifndef __COMMON_INSTRUMENTATION_SINKS_DEVNULL_H__
#define __COMMON_INSTRUMENTATION_SINKS_DEVNULL_H__

#include "instrumentation/sink.h"

namespace instrumentation {
namespace sinks {
class t_dev_null : public t_observation_sink {
public:
    DEFINE_BOOST_SHARED_PTRS(t_dev_null);

    inline bool operator << (const t_ptr<t_transaction_observation> & obs) {
        return true;
    }

    inline bool operator << (const t_ptr<t_oracle_observation> & obs) {
        return true;
    }

    inline bool operator << (const t_ptr<t_probe_observation> & obs) {
        return true;
    }
};
}
}

#endif