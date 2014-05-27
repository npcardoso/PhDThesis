#include <boost/test/unit_test.hpp>
#include <libdiag/structs/diagnosis_report.h>

#include <fstream>
using namespace diagnosis;
using namespace diagnosis::structs;

BOOST_AUTO_TEST_SUITE(Diagnosis_Report)
typedef t_candidate_generator::t_ret_type t_D;
typedef t_candidate_ranker::t_ret_type t_probs;

BOOST_AUTO_TEST_CASE(dr_next) {
    t_probs probs;
    t_D D;


    t_candidate c;
    // TODO
}


BOOST_AUTO_TEST_SUITE_END()