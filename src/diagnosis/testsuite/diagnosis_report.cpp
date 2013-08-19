#include <boost/test/unit_test.hpp>
#include "diagnosis/structs/diagnosis_report.h"

#include <fstream>
using namespace diagnosis;
using namespace diagnosis::structs;

BOOST_AUTO_TEST_SUITE(Diagnosis_Report)

BOOST_AUTO_TEST_CASE(dr_next) {
    t_diagnosis_report dr;
    t_candidate c;


    c.insert(1);
    dr.add(c, 4);

    c.insert(2);
    dr.add(c, 3);

    c.insert(3);
    dr.add(c, 2);

    c.clear();
    c.insert(4);
    dr.add(c, 1);

    c.clear();
    c.insert(3);
    dr.add(c, 0.5);

    c.clear();
    c.insert(1);
    BOOST_CHECK(dr.next(0, c) == 4);
    BOOST_CHECK(dr.next(4, c) == 5);
    BOOST_CHECK(dr.next(5, c) == 0);

    c.insert(4);
    BOOST_CHECK(dr.next(0, c) == 5);
    BOOST_CHECK(dr.next(5, c) == 0);

    c.clear();
    c.insert(2);
    BOOST_CHECK(dr.next(0, c) == 1);
    BOOST_CHECK(dr.next(1, c) == 4);
    BOOST_CHECK(dr.next(4, c) == 5);
    BOOST_CHECK(dr.next(5, c) == 0);
}


BOOST_AUTO_TEST_SUITE_END()