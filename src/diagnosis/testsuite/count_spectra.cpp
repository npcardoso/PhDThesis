#include <boost/test/unit_test.hpp>
#include "diagnosis/spectra/count_spectra.h"
#include "diagnosis/spectra/randomizer/bernoulli.h"

void check_equal (diagnosis::t_count_spectra & spectra, diagnosis::t_count_spectra & spectra2, int n_comp, int n_tran) {
    for (t_transaction_id t = 1; t <= n_tran; t++) {
        BOOST_CHECK(spectra.get_error(t) == spectra2.get_error(t));
        BOOST_CHECK(spectra.is_error(t) == spectra2.is_error(t));

        for (t_component_id c = 1; c <= n_comp; c++)
            BOOST_CHECK(spectra.get_count(c, t) == spectra2.get_count(c, t));
    }
}

BOOST_AUTO_TEST_SUITE(BasicSpectra)

BOOST_AUTO_TEST_CASE(size) {
    int n_comp = 10 + rand() % 20;
    int n_tran = 10 + rand() % 20;

    diagnosis::t_count_spectra spectra;


    BOOST_CHECK(spectra.get_component_count() == 0);
    BOOST_CHECK(spectra.get_transaction_count() == 0);
    BOOST_CHECK(spectra.get_error_count() == 0);

    diagnosis::t_bernoulli_randomizer randomizer(0.5, 0.5);
    randomizer.n_comp = n_comp;
    randomizer.n_tran = n_tran;
    randomizer.randomize(spectra);

    diagnosis::t_count_spectra spectra_half = spectra;
    spectra_half.set_element_count(n_comp / 2, n_tran / 2);

    BOOST_CHECK(spectra.get_component_count() == n_comp);
    BOOST_CHECK(spectra.get_transaction_count() == n_tran);
    BOOST_CHECK(spectra.get_error_count() >= spectra_half.get_error_count());
    BOOST_CHECK(spectra_half.get_component_count() == n_comp / 2);
    BOOST_CHECK(spectra_half.get_transaction_count() == n_tran / 2);

    check_equal(spectra, spectra_half, n_comp / 2, n_tran / 2);

    spectra_half.set_element_count(n_comp, n_tran);
    check_equal(spectra, spectra_half, n_comp / 2, n_tran / 2);

    spectra_half.set_count(1, 1, spectra.get_count(1, 1) - 1);
    BOOST_CHECK(spectra.get_count(1, 1) - 1 == spectra_half.get_count(1, 1));
}

BOOST_AUTO_TEST_CASE(error) {
    int n_comp = rand() % 500;
    int n_tran = rand() % 500;

    diagnosis::t_count_spectra spectra(n_comp, n_tran);


    for (int i = 1; i <= n_tran; i++) {
        spectra.error(i);
        BOOST_CHECK(spectra.is_error(i));
        BOOST_CHECK(spectra.get_error_count() == i);
    }

    for (int i = n_tran; i > 0; i--) {
        spectra.error(i, 0);
        BOOST_CHECK(spectra.is_error(i) == false);
        BOOST_CHECK(spectra.get_error_count() == i - 1);
    }
}

BOOST_AUTO_TEST_SUITE_END()