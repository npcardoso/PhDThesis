#include <boost/test/unit_test.hpp>

#include "structs/count_spectra.h"

#include <boost/foreach.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <sstream>

using namespace std;
using namespace diagnosis;
using namespace diagnosis::structs;

void check_equal (t_count_spectra & spectra, t_count_spectra & spectra2, t_count n_comp, t_count n_tran) {
    for (t_transaction_id t = 1; t <= n_tran; t++) {
        BOOST_CHECK(spectra.get_error(t) == spectra2.get_error(t));
        BOOST_CHECK(spectra.is_error(t) == spectra2.is_error(t));

        for (t_component_id c = 1; c <= n_comp; c++)
            BOOST_CHECK(spectra.get_activations(c, t) == spectra2.get_activations(c, t));
    }
}

void randomize (t_count_spectra & spectra) {
    t_spectra_iterator it(spectra.get_component_count(),
                          spectra.get_transaction_count());
    std::mt19937 gen;
    boost::random::uniform_int_distribution<t_count> rand(0, 100);


    while (it.transaction.next()) {
        while (it.component.next())
            spectra.hit(it.component.get(), it.transaction.get(), rand(gen));

        spectra.set_error(it.transaction.get(), rand(gen) / 100.0);
    }
}

BOOST_AUTO_TEST_SUITE(BasicSpectra)

BOOST_AUTO_TEST_CASE(is_candidate) {
    t_count_spectra spectra;
    stringstream ss("3 2  1 1 0 x  1 0 1 x");


    ss >> spectra;

    list<std::string> ok_candidates;
    list<std::string> not_ok_candidates;

    ok_candidates.push_back("1 0");
    ok_candidates.push_back("1 2 0");
    ok_candidates.push_back("1 3 0");
    ok_candidates.push_back("2 3 0");
    ok_candidates.push_back("1 2 3 0");
    not_ok_candidates.push_back("0");
    not_ok_candidates.push_back("2 0");
    not_ok_candidates.push_back("3 0");


    BOOST_FOREACH(std::string s, ok_candidates) {
        stringstream ss(s);
        t_candidate candidate;


        ss >> candidate;
        BOOST_CHECK(spectra.is_candidate(candidate));
    }

    BOOST_FOREACH(std::string s, not_ok_candidates) {
        stringstream ss(s);
        t_candidate candidate;


        ss >> candidate;
        BOOST_CHECK(!spectra.is_candidate(candidate));
    }
}

BOOST_AUTO_TEST_CASE(is_minimal_candidate) {
    t_count_spectra spectra;
    stringstream ss("3 2  1 1 0 x  1 0 1 x");


    ss >> spectra;

    list<std::string> ok_candidates;
    list<std::string> not_ok_candidates;

    ok_candidates.push_back("1 0");
    ok_candidates.push_back("2 3 0");
    not_ok_candidates.push_back("1 2 0");
    not_ok_candidates.push_back("1 3 0");
    not_ok_candidates.push_back("1 2 3 0");
    not_ok_candidates.push_back("0");
    not_ok_candidates.push_back("2 0");
    not_ok_candidates.push_back("3 0");


    BOOST_FOREACH(std::string s, ok_candidates) {
        stringstream ss(s);
        t_candidate candidate;


        ss >> candidate;
        BOOST_CHECK(spectra.is_minimal_candidate(candidate));
    }

    BOOST_FOREACH(std::string s, not_ok_candidates) {
        stringstream ss(s);
        t_candidate candidate;


        ss >> candidate;
        BOOST_CHECK(!spectra.is_minimal_candidate(candidate));
    }
}


BOOST_AUTO_TEST_CASE(size) {
    t_count n_comp = 10 + rand() % 20;
    t_count n_tran = 10 + rand() % 20;

    t_count_spectra spectra;
    t_candidate correct;
    std::mt19937 gen;


    BOOST_CHECK(spectra.get_component_count() == 0);
    BOOST_CHECK(spectra.get_transaction_count() == 0);
    BOOST_CHECK(spectra.get_error_count() == 0);


    spectra = t_count_spectra(n_comp, n_tran);
    randomize(spectra);

    t_count_spectra spectra_half = spectra;
    spectra_half.set_count(n_comp / 2, n_tran / 2);

    BOOST_CHECK_MESSAGE(spectra.get_component_count() == n_comp, "get_component_count() == " << spectra.get_component_count() << ", n_comp: " << n_comp);
    BOOST_CHECK_MESSAGE(spectra.get_transaction_count() == n_tran, "get_transaction_count() == " << spectra.get_transaction_count() << ", n_tran: " << n_tran);
    BOOST_CHECK(spectra.get_error_count() >= spectra_half.get_error_count());
    BOOST_CHECK(spectra_half.get_component_count() == n_comp / 2);
    BOOST_CHECK(spectra_half.get_transaction_count() == n_tran / 2);

    check_equal(spectra, spectra_half, n_comp / 2, n_tran / 2);

    spectra_half.set_count(n_comp, n_tran);
    check_equal(spectra, spectra_half, n_comp / 2, n_tran / 2);

    spectra_half.set_activations(1, 1, spectra.get_activations(1, 1) - 1);
    BOOST_CHECK(spectra.get_activations(1, 1) - 1 == spectra_half.get_activations(1, 1));
}

BOOST_AUTO_TEST_CASE(error) {
    t_count n_comp = rand() % 500;
    t_count n_tran = rand() % 500;

    t_count_spectra spectra(n_comp, n_tran);


    for (t_count i = 1; i <= n_tran; i++) {
        spectra.set_error(i, 1);
        BOOST_CHECK(spectra.is_error(i));
        BOOST_CHECK(spectra.get_error_count() == i);
    }

    for (t_count i = n_tran; i > 0; i--) {
        spectra.set_error(i, 0);
        BOOST_CHECK(spectra.is_error(i) == false);
        BOOST_CHECK(spectra.get_error_count() == i - 1);
    }
}

BOOST_AUTO_TEST_CASE(io) {
    t_count_spectra spectra;

    stringstream s;


    s.str("1 9\
           1 1\
           1 -\
           1 X\
           1 x\
           1 0.75\
           1 0.25\
           1 +\
           1 .\
           1 0\
           ");
    s >> spectra;
    BOOST_CHECK(s.good());
    BOOST_CHECK(spectra.get_component_count() == 1);
    BOOST_CHECK(spectra.get_transaction_count() == 9);

    for (t_id i = 1; i <= 4; i++) {
        BOOST_CHECK_MESSAGE(spectra.is_error(i), "Failed for transaction " << i);
        BOOST_CHECK_MESSAGE(spectra.get_error(i) == 1, "Failed for transaction " << i);
    }

    BOOST_CHECK_MESSAGE(!spectra.is_error(5), "Failed for transaction " << 5);
    BOOST_CHECK_MESSAGE(spectra.get_error(5) == 0.75, "Failed for transaction " << 5);

    BOOST_CHECK_MESSAGE(!spectra.is_error(6), "Failed for transaction " << 6);
    BOOST_CHECK_MESSAGE(spectra.get_error(6) == 0.25, "Failed for transaction " << 6);

    for (t_id i = 7; i <= 9; i++) {
        BOOST_CHECK_MESSAGE(spectra.is_error(i) == 0, "Failed for transaction " << i);
        BOOST_CHECK_MESSAGE(spectra.get_error(i) == 0, "Failed for transaction " << i);
    }
}

BOOST_AUTO_TEST_SUITE_END()