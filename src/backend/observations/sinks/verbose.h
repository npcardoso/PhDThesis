#ifndef __VERBOSE_H_7d9a2d112ecf975497256404f99ef717ba61d1e7__
#define __VERBOSE_H_7d9a2d112ecf975497256404f99ef717ba61d1e7__

#include "../sink.h"

#include <iostream>

class t_verbose : public t_sink {
    public:
    t_verbose(std::ostream & out = std::cerr);

    virtual bool transaction_start (t_artifact_id id,
                                    t_time_interval time);
    virtual bool transaction_end (t_artifact_id id,
                                  t_time_interval time);

//Probe with state
    virtual bool probe_init(t_artifact_id id,
                            t_time_interval time);
    virtual bool probe_read_state(void * ptr,
                                  t_count size);
    virtual bool probe_commit();


    virtual bool oracle(t_artifact_id id,
                        t_time_interval time,
                        t_error error,
                        t_confidence confidence);
    private:
    std::ostream & out;
};


#endif
