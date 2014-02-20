#ifndef __COMMON_INSTRUMENTATION_SINKS_THREAD_SAFE_H__
#define __COMMON_INSTRUMENTATION_SINKS_THREAD_SAFE_H__

#include "instrumentation/sink.h"
#include <boost/thread/mutex.hpp>

namespace instrumentation {
namespace sinks {
class t_thread_safe_construct : public t_construct_sink {
    boost::mutex mutex;
    t_ptr<t_construct_sink> sink;

    template <class CTR>
    bool protect (CTR & ctr) {
        mutex.lock();
        bool ret = (*sink) << ctr;
        mutex.unlock();
        return ret;
    }

public:
    t_thread_safe_construct (t_ptr<t_construct_sink> sink);

    virtual bool operator << (const t_ptr<t_transaction_construct> & ctr);
    virtual bool operator << (const t_ptr<t_oracle_construct> & ctr);
    virtual bool operator << (const t_ptr<t_probe_construct> & ctr);
};

class t_thread_safe_observation : public t_observation_sink {
    boost::mutex mutex;
    t_ptr<t_observation_sink> sink;

    template <class OBS>
    bool protect (OBS & obs) {
        mutex.lock();
        bool ret = (*sink) << obs;
        mutex.unlock();
        return ret;
    }

public:
    t_thread_safe_observation (t_ptr<t_observation_sink> sink);

    virtual bool operator << (const t_ptr<t_transaction_observation> & obs);
    virtual bool operator << (const t_ptr<t_oracle_observation> & obs);
    virtual bool operator << (const t_ptr<t_probe_observation> & obs);
};
}
}

#endif