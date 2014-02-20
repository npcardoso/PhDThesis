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
    virtual bool operator << (const t_ptr<t_transaction_observation> & obs);
    virtual bool operator << (const t_ptr<t_oracle_observation> & obs);
    virtual bool operator << (const t_ptr<t_probe_observation> & obs);
};

class t_construct_sink {
protected:
    inline t_construct_sink () {}
public:
    virtual bool operator << (const t_ptr<t_transaction_construct> & obs);
    virtual bool operator << (const t_ptr<t_oracle_construct> & obs);
    virtual bool operator << (const t_ptr<t_probe_construct> & obs);
};
}

#endif