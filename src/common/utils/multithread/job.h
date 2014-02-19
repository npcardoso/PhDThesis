#ifndef __JOB_H_464402f96b97d64e74d829d6fdf12e42f0f4450e__
#define __JOB_H_464402f96b97d64e74d829d6fdf12e42f0f4450e__

class t_job {
public:
    virtual void operator () () const = 0;
    virtual bool operator < (const t_job & job) const = 0;
    inline virtual ~t_job () {};
};

#endif