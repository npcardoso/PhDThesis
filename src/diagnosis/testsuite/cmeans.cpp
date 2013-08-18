#include <boost/test/unit_test.hpp>

#include "diagnosis/algorithms/cmeans.h"
#include "diagnosis/structs/count_spectra.h"
#include "diagnosis/structs/membership.h"

#include <iostream>
#include <fstream>

using namespace diagnosis;
using namespace diagnosis::structs;

BOOST_AUTO_TEST_SUITE(CMEANS)

BOOST_AUTO_TEST_CASE(with_spectra) {
    t_count_spectra spectra;
    std::stringstream s("2 2\
                      1 0 1\
                      0 1 1");


    s >> spectra;
    algorithms::t_cmeans cm(spectra);
    structs::t_membership memberships = cm.clustering(2);

    BOOST_CHECK(memberships.get_point_count() == 2);
    BOOST_CHECK(memberships.get_centroid_count() == 2);
    BOOST_CHECK(memberships.get_membership(1, 1) > memberships.get_membership(1, 2));
    BOOST_CHECK(memberships.hard_cluster(1) == 1);
    BOOST_CHECK(memberships.get_membership(2, 2) > memberships.get_membership(2, 1));
    BOOST_CHECK(memberships.hard_cluster(2) == 2);

    s.str("3 3\
        1 1 0 1\
        1 0 0 1\
        0 0 1 0");
    s.clear();
    s >> spectra;
    algorithms::t_cmeans cm2(spectra);
    structs::t_membership memberships2 = cm2.clustering();

    BOOST_CHECK(memberships2.get_point_count() == 3);
    BOOST_CHECK(memberships2.get_centroid_count() == 2);
    BOOST_CHECK(memberships2.get_hard_cluster_count(1) == 2);
    BOOST_CHECK(memberships2.hard_cluster(1) == 1);
    BOOST_CHECK(memberships2.hard_cluster(2) == 1);
    BOOST_CHECK(memberships2.get_hard_cluster_count(2) == 1);
    BOOST_CHECK(memberships2.hard_cluster(3) == 2);
}

BOOST_AUTO_TEST_SUITE_END()