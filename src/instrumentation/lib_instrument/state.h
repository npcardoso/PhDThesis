#ifndef __COMMON_INSTRUMENTATION_STATE_H__
#define __COMMON_INSTRUMENTATION_STATE_H__

#include "types.h"

namespace instrumentation {
class t_state {
public:
    t_state ();
    t_state (const t_state & s);
    ~t_state ();

    void read (const void * var,
               t_count bytes);
    template <class T>
    void read (const T & var) {
        read(&var, sizeof(var));
    }

    const t_count & size () const;

    const void * ptr (t_id pos) const;
    t_count width (t_id pos) const;

    template <class T>
    const T & get (t_id pos) {
        return *((const T *) ptr(pos));
    }

private:
    unsigned char * data;
    t_count * offset_end;
    t_count n_vars;
};
}

#endif