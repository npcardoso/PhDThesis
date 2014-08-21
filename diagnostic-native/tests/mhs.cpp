#include <boost/test/unit_test.hpp>
#include <libdiag/algorithms/mhs.h>
#include <libdiag/structs/count_spectrum.h>


#include <boost/foreach.hpp>
#include <fstream>
using namespace diagnostic;
using namespace diagnostic::algorithms;
using namespace diagnostic::structs;


struct F {
    F () {BOOST_TEST_MESSAGE("setup fixture");prepare_spectrums();prepare_D_refs();}
    ~F () {BOOST_TEST_MESSAGE("teardown fixture");}


    void prepare_spectrums () {
        std::ifstream f("io/mhs/in.txt");


        while (true) {
            spectrums.push_back(t_count_spectrum());
            t_count_spectrum & spectrum = *spectrums.rbegin();

            f >> spectrum;

            if (f.eof())
                break;

            BOOST_CHECK(spectrum.get_component_count() == 30);
            BOOST_CHECK(spectrum.get_transaction_count() == 30);
        }

        spectrums.pop_back();
        BOOST_REQUIRE(spectrums.size() == 30);
    }

    void prepare_D_refs () {
        std::ifstream f("io/mhs/out.txt");


        while (!f.eof()) {
            D_refs.push_back(t_trie());
            t_trie & D_ref = *D_refs.rbegin();

            f >> D_ref;
        }

        D_refs.pop_back();

        BOOST_REQUIRE(D_refs.size() == 30);
    }

    typedef std::list<t_count_spectrum> t_spectrums;
    typedef std::list<t_trie> t_D_refs;
    t_spectrums spectrums;
    t_D_refs D_refs;
};

BOOST_FIXTURE_TEST_SUITE(MHS2, F)
BOOST_AUTO_TEST_CASE(cutoff_depth) {
    auto cutoff = t_ptr<t_cutoff> (new t_cutoff());


    cutoff->max_depth = 4;

    t_mhs mhs;
    mhs.set_cutoff(cutoff);

    BOOST_FOREACH(const t_count_spectrum &spectrum, spectrums) {
        t_trie D;


        mhs(spectrum, D);

        BOOST_CHECK(D.size() != 0);

        BOOST_FOREACH(auto & d, D) {
            BOOST_CHECK(d.size() <= cutoff->max_depth);
        }
    }
}

BOOST_AUTO_TEST_CASE(cutoff_max_candidates) {
    auto cutoff = t_ptr<t_cutoff> (new t_cutoff());


    cutoff->max_candidates = 100;

    t_mhs mhs;
    mhs.set_cutoff(cutoff);

    BOOST_FOREACH(const t_count_spectrum &spectrum, spectrums) {
        t_trie D;


        mhs(spectrum, D);

        BOOST_CHECK(D.size() != 0);

        BOOST_CHECK_EQUAL(D.size(), cutoff->max_candidates);
    }
}


BOOST_AUTO_TEST_CASE(mhs) {
    t_D_refs::iterator D_ref = D_refs.begin();


    BOOST_FOREACH(const t_count_spectrum &spectrum, spectrums) {
        algorithms::t_mhs mhs;
        t_trie D;


        mhs(spectrum, D);
        BOOST_CHECK(D == *D_ref);
        D_ref++;
    }
}

template <class T>
void test_parallelization (F::t_spectrums & spectrums,
                           F::t_D_refs & D_refs,
                           t_count max_depth,
                           t_count max_threads) {
    F::t_D_refs::iterator D_ref = D_refs.begin();


    BOOST_FOREACH(const t_count_spectrum &spectrum, spectrums) {
        for (t_count depth = 1; depth < max_depth; depth++) {
            for (t_count n_threads = 1; n_threads < max_threads; n_threads++) {
                t_ptr<t_parallelization_factory> pf(new T(depth));
                algorithms::t_mhs_parallel mhs(pf, n_threads);
                diagnostic::t_trie D;

                mhs(spectrum, D);
                BOOST_CHECK_EQUAL(D.size(), D_ref->size());
                BOOST_CHECK_MESSAGE(D == *D_ref, "Failed for depth = " << depth << " n_threads = " << n_threads);
            }
        }

        D_ref++;
    }
}

BOOST_AUTO_TEST_CASE(mhs_parallel_random) {
    test_parallelization<t_parallelization_factory_random> (spectrums, D_refs, 4, 4);
}

BOOST_AUTO_TEST_CASE(mhs_parallel_stride) {
    test_parallelization<t_parallelization_factory_stride> (spectrums, D_refs, 4, 4);
}


BOOST_AUTO_TEST_SUITE_END()
