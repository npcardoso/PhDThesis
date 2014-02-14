#ifndef __JOB_H_464402f96b97d64e74d829d6fdf12e42f0f4450e__
#define __JOB_H_464402f96b97d64e74d829d6fdf12e42f0f4450e__


#include "utils/boost.h"
#include "utils/time.h"

class t_job {
public:
    virtual void operator () () const = 0;
    virtual std::string get_type () const = 0;
    virtual bool operator < (const t_job & job) const = 0;
    inline virtual ~t_job () {};
};

#else

class t_job;
// class t_queued_job;

#endif