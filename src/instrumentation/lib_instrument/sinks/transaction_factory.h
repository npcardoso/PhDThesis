#ifndef __COMMON_INSTRUMENTATION_SINKS_TRANSACTION_FACTORY_H__
#define __COMMON_INSTRUMENTATION_SINKS_TRANSACTION_FACTORY_H__

#include "instrumentation/sink.h"

#include "types.h"

#include <map>
#include <stack>
#include <string>

namespace instrumentation {
namespace sinks {
class t_transaction_factory : public t_observation_sink {
    typedef std::stack<t_ptr<t_transaction_observation> > t_stack;
    t_stack transactions;
    t_ptr<t_observation_sink> sink;

public:
    t_transaction_factory (t_ptr<t_observation_sink> sink=t_ptr<t_observation_sink> ());

    size_t num_active () const;

    virtual bool operator << (const t_ptr<t_transaction_observation> & obs);

    virtual bool operator << (const t_ptr<t_oracle_observation> & obs);

    virtual bool operator << (const t_ptr<t_probe_observation> & obs);
};
}
}

#endif