#ifndef __TRACKER_H_1868c3c4a754c18096c9eb06264b3497b499724c__
#define __TRACKER_H_1868c3c4a754c18096c9eb06264b3497b499724c__
#include "sink.h"
#include "../common.h"

#include <map>
#include <mutex>


class t_tracker {
public:
    typedef t_sink * (* t_sink_gen)(void);

    t_tracker (t_sink_gen sink_gen, t_sink_gen auto_sink_gen);
    inline virtual ~t_tracker () {}

    virtual void start (t_thread_id thread_id=thread_self()) = 0;
    virtual void end (t_thread_id thread_id=thread_self()) = 0;
    virtual t_sink & get (t_thread_id thread_id=thread_self()) = 0;

protected:
    t_sink_gen sink_gen;
    t_sink_gen auto_sink_gen;
};

class t_tracker_singlethread : public t_tracker {
public:
    t_tracker_singlethread (t_sink_gen sink_gen, t_sink_gen auto_sink_gen);
    virtual ~t_tracker_singlethread ();

    virtual void start (t_thread_id thread_id=thread_self());
    virtual void end (t_thread_id thread_id=thread_self());
    t_sink & get (t_thread_id thread_id=thread_self());

private:
    t_sink * sink;
};

class t_tracker_multithread : public t_tracker {
public:
    t_tracker_multithread (t_sink_gen sink_gen, t_sink_gen auto_sink_gen);
    virtual ~t_tracker_multithread ();

    void start (t_thread_id thread_id=thread_self());
    void end (t_thread_id thread_id=thread_self());
    t_sink & get (t_thread_id thread_id=thread_self());


private:
    typedef std::map<t_thread_id, t_tracker_singlethread *> t_sinks;
    std::mutex mutex;
    t_sinks sinks;
};

#endif