#ifndef __DIAGNOSTIC_SYSTEM_H_2d027fe833f058443bd53e5b08fdd5e63b74e055__
#define __DIAGNOSTIC_SYSTEM_H_2d027fe833f058443bd53e5b08fdd5e63b74e055__

#include "types.h"
#include "connection.h"
#include "structs/spectra.h"
#include "structs/spectra_filter.h"
#include "structs/trie.h"
#include "utils/boost.h"

#include <list>
#include <ostream>

namespace diagnostic {
class t_candidate_generator {
public:
    typedef t_trie t_ret_type;

    virtual void operator () (const t_spectra & spectra,
                              t_ret_type & D,
                              const t_spectra_filter * filter=NULL) const = 0;
    virtual std::string to_string() const = 0;
    inline virtual ~t_candidate_generator () {}
};


class t_candidate_ranker {
public:
    class t_ret_type : public std::list<t_probability> {
public:
    };

    virtual void operator () (const t_spectra & spectra,
                              const t_trie & D,
                              t_ret_type & probs,
                              const t_spectra_filter * filter=NULL) const = 0;

    virtual t_score_type get_score_type () const = 0;
    virtual std::string to_string() const = 0;
    inline virtual ~t_candidate_ranker () {}
};

class t_diagnostic_system {
public:

    typedef std::vector<t_connection> t_connections;
    typedef std::vector <t_const_ptr < t_candidate_generator >> t_generators;
    typedef std::vector <t_const_ptr < t_candidate_ranker >> t_rankers;


    void add_generator (t_const_ptr<t_candidate_generator> generator);
    void add_generator (t_const_ptr<t_candidate_generator> generator,
                        std::string name);


    const t_generators & get_generators () const;

    t_id get_generator_id (std::string name) const;
    std::string get_generator_name (t_id generator_id) const;



    void add_ranker (t_const_ptr<t_candidate_ranker> ranker);
    void add_ranker (t_const_ptr<t_candidate_ranker> ranker,
                     std::string name);

    const t_rankers & get_rankers() const;

    t_id get_ranker_id (std::string name) const;
    std::string get_ranker_name (t_id ranker_id) const;



    void add_connection (const t_connection & c);
    void add_connection (t_id generator_id,
                         t_id ranker_id);
    void add_connection (std::string generator_id,
                         std::string ranker_id);

    const t_connections & get_connections () const;

private:
    typedef std::map<std::string, t_id> t_id_map;
    t_id_map generator_ids;
    t_id_map ranker_ids;

    typedef std::vector<std::string> t_name_vector;
    t_name_vector generator_names;
    t_name_vector ranker_names;

    t_generators generators;
    t_rankers rankers;
    t_connections connections;
};
}

namespace std {
std::ostream& operator<<(std::ostream & s,
                         const diagnostic::t_candidate_generator & cg);
std::ostream& operator<<(std::ostream & s,
                         const diagnostic::t_candidate_ranker & cr);
std::ostream& operator<<(std::ostream& s,
                         const diagnostic::t_diagnostic_system & ds);
}

#endif
