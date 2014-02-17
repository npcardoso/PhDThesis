#include <boost/test/unit_test.hpp>

#include "diagnosis/structs/count_spectra.h"
#include "diagnosis/benchmark/generators/bernoulli.h"

#include <sstream>

using namespace std;
using namespace diagnosis;
using namespace diagnosis::structs;
using namespace diagnosis::benchmark;

void check_equal (t_count_spectra & spectra, t_count_spectra & spectra2, int n_comp, int n_tran) {
    for (t_transaction_id t = 1; t <= n_tran; t++) {
        BOOST_CHECK(spectra.get_error(t) == spectra2.get_error(t));
        BOOST_CHECK(spectra.is_error(t) == spectra2.is_error(t));

        for (t_component_id c = 1; c <= n_comp; c++)
            BOOST_CHECK(spectra.get_activations(c, t) == spectra2.get_activations(c, t));
    }
}

BOOST_AUTO_TEST_SUITE(BasicSpectra)

BOOST_AUTO_TEST_CASE(size) {
    int n_comp = 10 + rand() % 20;
    int n_tran = 10 + rand() % 20;

    t_count_spectra * spectra = new t_count_spectra();
    t_candidate correct;
    std::mt19937 gen;


    BOOST_CHECK(spectra->get_component_count() == 0);
    BOOST_CHECK(spectra->get_transaction_count() == 0);
    BOOST_CHECK(spectra->get_error_count() == 0);

    delete spectra;


    t_bernoulli randomizer(0.5, 0.5, n_tran, n_comp);
    spectra = dynamic_cast<t_count_spectra *> (randomizer(gen, correct));

    t_count_spectra spectra_half = *spectra;
    spectra_half.set_count(n_comp / 2, n_tran / 2);

    BOOST_CHECK_MESSAGE(spectra->get_component_count() == n_comp, "get_component_count() == " << spectra->get_component_count() << ", n_comp: " << n_comp);
    BOOST_CHECK_MESSAGE(spectra->get_transaction_count() == n_tran, "get_transaction_count() == " << spectra->get_transaction_count() << ", n_tran: " << n_tran);
    BOOST_CHECK(spectra->get_error_count() >= spectra_half.get_error_count());
    BOOST_CHECK(spectra_half.get_component_count() == n_comp / 2);
    BOOST_CHECK(spectra_half.get_transaction_count() == n_tran / 2);

    check_equal(*spectra, spectra_half, n_comp / 2, n_tran / 2);

    spectra_half.set_count(n_comp, n_tran);
    check_equal(*spectra, spectra_half, n_comp / 2, n_tran / 2);

    spectra_half.set_activations(1, 1, spectra->get_activations(1, 1) - 1);
    BOOST_CHECK(spectra->get_activations(1, 1) - 1 == spectra_half.get_activations(1, 1));
    delete spectra;
}

BOOST_AUTO_TEST_CASE(error) {
    int n_comp = rand() % 500;
    int n_tran = rand() % 500;

    t_count_spectra spectra(n_comp, n_tran);


    for (int i = 1; i <= n_tran; i++) {
        spectra.set_error(i, 1);
        BOOST_CHECK(spectra.is_error(i));
        BOOST_CHECK(spectra.get_error_count() == i);
    }

    for (int i = n_tran; i > 0; i--) {
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

    s.str("1 9\
           1 1 0.1\
           1 - 0.2\
           1 X 0.3\
           1 x 0.4\
           1 0.75 0.5\
           1 0.25 0.6\
           1 + 0.7\
           1 . 0.8\
           1 0 0.9\
           ");
    spectra.read(s, true);
    BOOST_CHECK(s.good());
    BOOST_CHECK(spectra.get_component_count() == 1);
    BOOST_CHECK(spectra.get_transaction_count() == 9);

    for (t_id i = 1; i <= 9; i++) {
        BOOST_CHECK_MESSAGE(spectra.get_confidence(i) - i / 10.0 < 0.00001, "Failed for transaction " << i);
    }

    for (t_id i = 1; i <= 4; i++) {
        BOOST_CHECK_MESSAGE(!spectra.is_error(i), "Failed for transaction " << i);
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