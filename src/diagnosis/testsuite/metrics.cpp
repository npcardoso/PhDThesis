#include <boost/test/unit_test.hpp>
#include "diagnosis/metrics.h"

#include <fstream>
using namespace diagnosis;
using namespace diagnosis::metrics;

BOOST_AUTO_TEST_SUITE(Metrics)

BOOST_AUTO_TEST_CASE(C_d) {
    typedef diagnosis::t_rank_element<float, t_component_id> t_rank_element;
    std::list<t_rank_element> l;

    l.push_back(t_rank_element(1, -1));

    BOOST_CHECK_CLOSE(Cd(1, l), 0, 0.00001);
    BOOST_CHECK_CLOSE(quality(Cd(1, l), l.size()), 1, 0.00001);
    BOOST_CHECK(isnan(Cd(2, l)));

    l.push_back(t_rank_element(2, -2));
    l.push_back(t_rank_element(21, -2));
    l.push_back(t_rank_element(22, -2));

    BOOST_CHECK_CLOSE(Cd(1, l), 0, 0.00001);
    BOOST_CHECK_CLOSE(quality(Cd(1, l), l.size()), 1, 0.00001);
    BOOST_CHECK_CLOSE(Cd(2, l), 2, 0.00001);
    BOOST_CHECK_CLOSE(quality(Cd(2, l), l.size()), 1 - (2 / 3.0), 0.00001);
    BOOST_CHECK(isnan(Cd(3, l)));

    l.push_back(t_rank_element(3, -3));
    l.push_back(t_rank_element(31, -3));
    l.push_back(t_rank_element(32, -3));
    l.push_back(t_rank_element(33, -3));
    l.push_back(t_rank_element(34, -3));
    l.push_back(t_rank_element(35, -3));
    l.push_back(t_rank_element(36, -3));

    BOOST_CHECK_CLOSE(Cd(1, l), 0, 0.00001);
    BOOST_CHECK_CLOSE(quality(Cd(1, l), l.size()), 1, 0.00001);
    BOOST_CHECK_CLOSE(Cd(2, l), 2, 0.00001);
    BOOST_CHECK_CLOSE(quality(Cd(2, l), l.size()), 1 - (2 / 10.0), 0.00001);
    BOOST_CHECK_CLOSE(Cd(3, l), 7, 0.00001);
    BOOST_CHECK_CLOSE(quality(Cd(3, l), l.size()), 1 - (7 / 10.0), 0.00001);
    BOOST_CHECK(isnan(Cd(4, l)));
}

BOOST_AUTO_TEST_SUITE_END()