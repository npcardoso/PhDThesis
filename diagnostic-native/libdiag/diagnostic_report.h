#ifndef __DIAGNOSTIC_REPORT_H_132a205ec90bbf9374e48a44e208119ba72231fa__
#define __DIAGNOSTIC_REPORT_H_132a205ec90bbf9374e48a44e208119ba72231fa__

#include "serializable.h"
#include "diagnostic_system.h"
#include "ranking.h"

#include <map>


namespace diagnostic {

class t_diagnostic_report : public json_serializable {
    public:
    t_diagnostic_report(const t_const_ptr<t_diagnostic_system> & diagnostic_system);

    t_ranking * create_ranking(t_connection c) const;


    void add(t_id generator_id,
             t_const_ptr <t_candidate_generator::t_ret_type> D);

    void add (const t_connection & c,
              t_const_ptr<t_candidate_ranker::t_ret_type> scores);

    inline const t_const_ptr<t_diagnostic_system> & get_diagnostic_system() const {
        return diagnostic_system;
    }

    std::ostream & json_write(std::ostream & out) const;

    private:
    t_const_ptr<t_diagnostic_system> diagnostic_system;

    std::map<t_id, t_const_ptr<t_candidate_generator::t_ret_type>> candidate_sets;
    std::map<t_connection, t_const_ptr<t_candidate_ranker::t_ret_type> > scores;
};
}


namespace std {
std::ostream & operator << (std::ostream & out,
                            const diagnostic::t_diagnostic_report & dr);
}


#endif
