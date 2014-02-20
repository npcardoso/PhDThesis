#include "instrumentation/sink.h"
namespace instrumentation {
bool t_observation_sink::operator << (const t_ptr<t_transaction_observation> & obs) {
    return false;
}

bool t_observation_sink::operator << (const t_ptr<t_oracle_observation> & obs) {
    return false;
}

bool t_observation_sink::operator << (const t_ptr<t_probe_observation> & obs) {
    return false;
}

bool t_construct_sink::operator << (const t_ptr<t_transaction_construct> & obs) {
    return false;
}

bool t_construct_sink::operator << (const t_ptr<t_oracle_construct> & obs) {
    return false;
}

bool t_construct_sink::operator << (const t_ptr<t_probe_construct> & obs) {
    return false;
}
}