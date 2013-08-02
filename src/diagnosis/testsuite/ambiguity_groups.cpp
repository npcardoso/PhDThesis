#include <boost/test/unit_test.hpp>
#include "diagnosis/spectra/ambiguity_groups.h"
#include "diagnosis/spectra/count_spectra.h"

#include <fstream>
using namespace diagnosis;
using namespace std;

BOOST_AUTO_TEST_SUITE(AMBIGUITY_GROUPS)

BOOST_AUTO_TEST_CASE(AG1) {
    t_count_spectra spectra;
    t_ambiguity_groups ag(spectra);
    stringstream s;


    s.str("2 2\
           1 1 1\
           1 1 1");
    s >> spectra;
    ag = t_ambiguity_groups(spectra);

    BOOST_CHECK(!ag.filter().is_component(1));
    BOOST_CHECK(ag.filter().is_component(2));
    BOOST_CHECK(ag.group(1).count(2));
    BOOST_CHECK(ag.group(1).size() == 1);

    s.str("3 2\
           1 1 0 1\
           1 1 1 1");
    spectra = t_count_spectra();
    s >> spectra;
    ag = t_ambiguity_groups(spectra);

    BOOST_CHECK(!ag.filter().is_component(1));
    BOOST_CHECK(ag.filter().is_component(2));
    BOOST_CHECK(!ag.filter().is_component(3));
    BOOST_CHECK(ag.group(1).count(2));
    BOOST_CHECK(ag.group(1).size() == 1);

    s.str("10 3\
           0 1 0 0 1 0 1 1 1 1 1\
           1 1 1 1 1 1 0 0 1 0 1\
           1 1 1 0 1 0 1 0 1 1 1");
    spectra = t_count_spectra();
    s >> spectra;
    std::cout << spectra;
    ag = t_ambiguity_groups(spectra);
    spectra.write(std::cout, &ag.filter());

    BOOST_CHECK(!ag.filter().is_component(1));
    BOOST_CHECK(!ag.filter().is_component(2));
    BOOST_CHECK(ag.filter().is_component(3));
    BOOST_CHECK(!ag.filter().is_component(4));
    BOOST_CHECK(ag.filter().is_component(5));
    BOOST_CHECK(ag.filter().is_component(6));
    BOOST_CHECK(!ag.filter().is_component(7));
    BOOST_CHECK(!ag.filter().is_component(8));
    BOOST_CHECK(ag.filter().is_component(9));
    BOOST_CHECK(ag.filter().is_component(10));

    BOOST_CHECK(ag.group(1).count(3));
    BOOST_CHECK(ag.group(1).size() == 1);

    BOOST_CHECK(ag.group(2).count(5));
    BOOST_CHECK(ag.group(2).count(9));
    BOOST_CHECK(ag.group(2).size() == 2);

    BOOST_CHECK(ag.group(4).count(6));
    BOOST_CHECK(ag.group(4).size() == 1);

    BOOST_CHECK(ag.group(7).count(10));
    BOOST_CHECK(ag.group(7).size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()