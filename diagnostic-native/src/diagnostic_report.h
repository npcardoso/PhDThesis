#ifndef __DIAGNOSTIC_REPORT_H_132a205ec90bbf9374e48a44e208119ba72231fa__
#define __DIAGNOSTIC_REPORT_H_132a205ec90bbf9374e48a44e208119ba72231fa__

#include "diagnostic_system.h"
#include "report/ranking.h"

#include <vector>


namespace diagnostic {

class t_diagnostic_report {
    public:
    typedef std::vector<t_const_ptr<t_candidate_generator::t_ret_type>> t_gen_results;
    typedef std::vector<t_const_ptr<t_candidate_ranker::t_ret_type> > t_rank_results;

    t_diagnostic_report(t_const_ptr<t_diagnostic_system> diagnostic_system);

    t_ranking * create_ranking(t_id connection_id) const;


    void add(t_id generator_id,
             t_const_ptr <t_candidate_generator::t_ret_type> result);

    void add (t_id connection_id,
              t_const_ptr<t_candidate_ranker::t_ret_type> result);

    t_const_ptr<t_diagnostic_system> get_diagnostic_system() const;
    const t_gen_results & get_generator_results () const;
    const t_rank_results & get_ranker_results() const;

    private:
    t_const_ptr<t_diagnostic_system> diagnostic_system;
    t_gen_results gen_results;
    t_rank_results rank_results;
};
}


#endif
