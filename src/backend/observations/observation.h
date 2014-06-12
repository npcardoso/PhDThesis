#ifndef __OBSERVATION_H_bf6ea2a12001dc41f185cb3b2b3570f948bd5449__
#define __OBSERVATION_H_bf6ea2a12001dc41f185cb3b2b3570f948bd5449__

#include "../types.h"

class t_observation {
public:
    virtual bool operator > (const t_time_interval & time) const = 0;
    virtual bool operator < (const t_time_interval & time) const = 0;
};

class t_observation_single : public t_observation {
protected:
    t_observation_single ();
    t_observation_single (t_artifact_id id,
                          t_time_interval time);

public:
    virtual bool operator > (const t_time_interval & time) const;
    virtual bool operator < (const t_time_interval & time) const;

    t_artifact_id id;
    t_time_interval time;
};

class t_observation_window : public t_observation {
protected:
    t_observation_window ();
    t_observation_window (t_artifact_id id_start,
                          t_time_interval time_start);
    t_observation_window (t_artifact_id id_start,
                          t_time_interval time_start,
                          t_artifact_id id_end,
                          t_time_interval time_end);

public:
    virtual bool ended () const;

    virtual bool operator > (const t_time_interval & time) const;
    virtual bool operator < (const t_time_interval & time) const;

    t_artifact_id id_start, id_end;
    t_time_interval time_start, time_end;
};

#endif
