#ifndef __DIAGNOSTIC_SYSTEM_H_2d027fe833f058443bd53e5b08fdd5e63b74e055__
#define __DIAGNOSTIC_SYSTEM_H_2d027fe833f058443bd53e5b08fdd5e63b74e055__

#include "types.h"
#include "structs/spectra.h"
#include "structs/spectra_filter.h"
#include "structs/trie.h"
#include "utils/boost.h"

#include <list>
#include <ostream>

namespace diagnostic{
class t_candidate_generator;
class t_candidate_ranker;
class t_diagnostic_system;
}

namespace std {
std::ostream& operator<<(std::ostream & s,
                         const diagnostic::t_candidate_generator & cg);
std::ostream& operator<<(std::ostream & s,
                         const diagnostic::t_candidate_ranker & cr);
std::ostream& operator<<(std::ostream& s,
                         const diagnostic::t_diagnostic_system & ds);
}

namespace diagnostic {
class t_candidate_generator {
public:
    typedef structs::t_trie t_ret_type;

    virtual void operator () (const structs::t_spectra & spectra,
                              t_ret_type & D,
                              const structs::t_spectra_filter * filter=NULL) const = 0;
    virtual std::string to_string() const = 0;
    inline virtual ~t_candidate_generator () {}
};


class t_candidate_ranker {
public:
    class t_ret_type : public std::list<t_probability_mp> {
public:
    };

    virtual void operator () (const structs::t_spectra & spectra,
                              const structs::t_trie & D,
                              t_ret_type & probs,
                              const structs::t_spectra_filter * filter=NULL) const = 0;
    virtual std::string to_string() const = 0;
    inline virtual ~t_candidate_ranker () {}
};

class t_diagnostic_system {
public:
    typedef std::list<t_id> t_ranker_list;

    void add_generator (const t_const_ptr<t_candidate_generator> & generator);
    void add_generator (const t_const_ptr<t_candidate_generator> & generator,
                        const std::string & name);

    void add_ranker (const t_const_ptr<t_candidate_ranker> & ranker);
    void add_ranker (const t_const_ptr<t_candidate_ranker> & ranker,
                     const std::string & name);

    const t_const_ptr<t_candidate_generator> & get_generator (t_id generator_id) const;
    t_id get_generator_id (const std::string & name) const;
    const std::string & get_generator_name (t_id generator_id) const;
    t_count get_generator_count () const;
    const t_ranker_list & get_connections (t_id generator_id) const;

    const t_const_ptr<t_candidate_ranker> & get_ranker (t_id ranker_id) const;
    t_id get_ranker_id (const std::string & name) const;
    const std::string & get_ranker_name (t_id ranker_id) const;

    void add_connection (t_id generator_id,
                         t_id ranker_id);
    void add_connection (const std::string & generator_id,
                         const std::string & ranker_id);

    friend std::ostream& std::operator<<(std::ostream& s, const t_diagnostic_system & ds);

protected:
    typedef std::vector<t_ranker_list> t_connections;
    t_connections connections;

    typedef std::map<std::string, t_id> t_id_map;
    t_id_map generator_ids;
    t_id_map ranker_ids;

    typedef std::vector<std::string> t_name_vector;
    t_name_vector generator_names;
    t_name_vector ranker_names;

    std::vector < t_const_ptr < t_candidate_generator >> generators;
    std::vector < t_const_ptr < t_candidate_ranker >> rankers;
};
}


#endif
