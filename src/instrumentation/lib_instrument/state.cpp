#include "instrumentation/state.h"

#include <cassert>
#include <cstdlib>
#include <cstring>

namespace instrumentation {
t_state::t_state () {
    n_vars = 0;
    data = NULL;
    offset_end = NULL;
}

t_state::t_state (const t_state & s) {
    n_vars = s.n_vars;
    data = NULL;
    offset_end = NULL;

    if (s.data && s.offset_end) {
        offset_end = (t_count *) malloc(n_vars * sizeof(t_count));
        memcpy(offset_end, s.offset_end, n_vars * sizeof(t_count));

        data = (unsigned char *) malloc(offset_end[n_vars - 1]);
        memcpy(data, s.data, offset_end[n_vars - 1]);
    }
}

void t_state::read (const void * var, t_count bytes) {
    assert(bytes > 0);

    t_count start;

    if (n_vars) {
        start = offset_end[n_vars - 1];
        offset_end = (t_count *) realloc(offset_end, (n_vars + 1) * sizeof(t_count));
        data = (unsigned char *) realloc(data, start + bytes);
    }
    else {
        start = 0;
        offset_end = (t_count *) malloc(sizeof(t_count));
        data = (unsigned char *) malloc(bytes);
    }

    assert(offset_end != NULL);
    assert(data != NULL);

    offset_end[n_vars] = start + bytes;
    memcpy(data + start, var, bytes);

    n_vars++;
}

const t_count & t_state::size () const {
    return n_vars;
}

const void * t_state::ptr (t_id pos) const {
    if (pos)
        return data + offset_end[pos - 1];
    else
        return data;
}

t_count t_state::width (t_id pos) const {
    if (pos)
        return offset_end[pos] - offset_end[pos - 1];
    else
        return offset_end[0];
}

t_state::~t_state () {
    if (data)
        free(data);

    if (offset_end)
        free(offset_end);
}
}