#ifndef __COMMON_INSTRUMENTATION_SINK_H__
#define __COMMON_INSTRUMENTATION_SINK_H__

#include "instrumentation/transaction.h"
#include "instrumentation/oracle.h"
#include "instrumentation/probe.h"
namespace instrumentation {
class t_observation_sink {
protected:
    inline t_observation_sink () {}

public:
    DEFINE_BOOST_SHARED_PTRS(t_observation_sink);

    virtual bool operator << (const t_transaction_observation::t_ptr & obs);
    virtual bool operator << (const t_oracle_observation::t_ptr & obs);
    virtual bool operator << (const t_probe_observation::t_ptr & obs);
};

class t_construct_sink {
protected:
    inline t_construct_sink () {}
public:
    DEFINE_BOOST_SHARED_PTRS(t_construct_sink);

    virtual bool operator << (const t_transaction_construct::t_ptr & obs);
    virtual bool operator << (const t_oracle_construct::t_ptr & obs);
    virtual bool operator << (const t_probe_construct::t_ptr & obs);
};
}

#endif