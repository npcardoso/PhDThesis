#include "instrumentation/sinks/thread_safe.h"

namespace instrumentation {
namespace sinks {
t_thread_safe_construct::t_thread_safe_construct (t_construct_sink::t_ptr sink) : sink(sink) {}

bool t_thread_safe_construct::operator << (const t_transaction_construct::t_ptr & ctr) {
    return protect(ctr);
}

bool t_thread_safe_construct::operator << (const t_oracle_construct::t_ptr & ctr) {
    return protect(ctr);
}

bool t_thread_safe_construct::operator << (const t_probe_construct::t_ptr & ctr) {
    return protect(ctr);
}

t_thread_safe_observation::t_thread_safe_observation (t_observation_sink::t_ptr sink) : sink(sink) {}

bool t_thread_safe_observation::operator << (const t_transaction_observation::t_ptr & obs) {
    return protect(obs);
}

bool t_thread_safe_observation::operator << (const t_oracle_observation::t_ptr & obs) {
    return protect(obs);
}

bool t_thread_safe_observation::operator << (const t_probe_observation::t_ptr & obs) {
    return protect(obs);
}
}
}