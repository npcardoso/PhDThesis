#ifndef __SINK_H_8350e7e27531e9bdb068d5a07c44b642ad2d8640__
#define __SINK_H_8350e7e27531e9bdb068d5a07c44b642ad2d8640__

#include "../types.h"

class t_sink {
protected:
    inline t_sink () {}
public:
    inline virtual ~t_sink() {}

    virtual bool transaction_start (t_artifact_id id,
                                    t_time_interval time) = 0;
    virtual bool transaction_end (t_artifact_id id,
                                  t_time_interval time) = 0;


// Probe without state
    virtual bool probe (t_artifact_id id,
                        t_time_interval time);

//Probe with state
    virtual bool probe_init(t_artifact_id id,
                            t_time_interval time) = 0;
    virtual bool probe_read_state(void * ptr,
                                  t_count size) = 0;
    virtual bool probe_commit() = 0;


    virtual bool oracle(t_artifact_id id,
                        t_time_interval time,
                        t_error error,
                        t_confidence confidence) = 0;
};

#endif
