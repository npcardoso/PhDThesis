#ifndef __DIAGNOSTIC_SYSTEM_H_2d027fe833f058443bd53e5b08fdd5e63b74e055__
#define __DIAGNOSTIC_SYSTEM_H_2d027fe833f058443bd53e5b08fdd5e63b74e055__

#include "candidate_generator.h"
#include "candidate_ranker.h"
#include "connection.h"
#include "utils/boost.h"
#include "utils/json.h"

#include <ostream>

namespace diagnostic {
class t_diagnostic_system : public t_json_writable {
public:

    typedef std::vector<t_connection> t_connections;
    typedef std::vector<t_const_ptr<t_candidate_generator> > t_generators;
    typedef std::vector<t_const_ptr<t_candidate_ranker> > t_rankers;


    void add_generator (t_const_ptr<t_candidate_generator> generator);
    const t_generators & get_generators () const;


    void add_ranker (t_const_ptr<t_candidate_ranker> ranker);
    const t_rankers & get_rankers () const;

    void add_connection (const t_connection & c);
    void add_connection (t_id generator_id,
                         t_id ranker_id);
    const t_connections & get_connections () const;

    virtual std::ostream & json (std::ostream & out) const;

private:
    t_generators generators;
    t_rankers rankers;
    t_connections connections;
};
}

namespace std {
std::ostream & operator << (std::ostream & s,
                            const diagnostic::t_diagnostic_system & ds);
}

#endif