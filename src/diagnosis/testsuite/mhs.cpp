#include <boost/test/unit_test.hpp>
#include "diagnosis/algorithms/mhs.h"
#include "diagnosis/structs/count_spectra.h"
#include "diagnosis/benchmark/generators/bernoulli.h"

#include <boost/foreach.hpp>
#include <fstream>
using namespace diagnosis;
using namespace diagnosis::algorithms;
using namespace diagnosis::structs;
using namespace diagnosis::benchmark;


struct F {
    F () {BOOST_TEST_MESSAGE("setup fixture");prepare_spectras();prepare_D_refs();}
    ~F () {BOOST_TEST_MESSAGE("teardown fixture");}


    void prepare_spectras () {
        for (t_id i = 0; i < 30; i++) {
            spectras.push_back(t_count_spectra());
            t_count_spectra & spectra = *spectras.rbegin();

            std::stringstream s;
            std::ifstream f;

            // Reading Input
            s << "io/mhs/in." << i << ".txt";
            f.open(s.str().c_str());

            f >> spectra;
            BOOST_CHECK(f.good());

            f.close();
        }
    }

    void prepare_D_refs () {
        for (t_id i = 0; i < 30; i++) {
            D_refs.push_back(t_trie());
            t_trie & D_ref = *D_refs.rbegin();

            std::stringstream s;
            std::ifstream f;

            // Reading Output
            s << "io/mhs/out." << i << ".txt";
            f.open(s.str().c_str());
            f >> D_ref;
            BOOST_CHECK(f.good());
            f.close();
        }
    }

    typedef std::list<t_count_spectra> t_spectras;
    typedef std::list<t_trie> t_D_refs;
    t_spectras spectras;
    t_D_refs D_refs;
};

BOOST_FIXTURE_TEST_SUITE(MHS2, F)
BOOST_AUTO_TEST_CASE(cutoff_depth) {
    t_spectra * spectra;
    t_candidate correct;
    t_bernoulli randomizer(0.25, 1, 6, 100);
    std::mt19937 gen;
    t_trie D;
    auto cutoff = t_ptr<t_cutoff> (new t_cutoff());
    t_mhs mhs;


    cutoff->max_depth = 2;
    mhs.set_cutoff(cutoff);

    spectra = randomizer(gen, correct);

    mhs(*spectra, D);

    // Max Candidate Size
    t_trie::iterator it = D.begin();

    BOOST_CHECK(D.size() != 0);

    while (it != D.end())
        BOOST_CHECK((it++)->size() <= cutoff->max_depth);

    delete spectra;
}

BOOST_AUTO_TEST_CASE(cutoff_max_candidates) {
    t_spectra * spectra;
    t_candidate correct;
    t_bernoulli randomizer(0.25, 1, 100, 100);
    std::mt19937 gen;
    t_trie D;
    auto cutoff = t_ptr<t_cutoff> (new t_cutoff());
    t_mhs mhs;


    cutoff->max_candidates = 100;
    mhs.set_cutoff(cutoff);

    spectra = randomizer(gen, correct);
    mhs(*spectra, D);


    // Max Candidate Size
    mhs(*spectra, D);
    BOOST_CHECK_EQUAL(D.size(), cutoff->max_candidates);

    delete spectra;
}


BOOST_AUTO_TEST_CASE(mhs) {
    t_D_refs::iterator D_ref = D_refs.begin();


    BOOST_FOREACH(const t_count_spectra &spectra, spectras) {
        algorithms::t_mhs mhs;
        t_trie D;


        mhs(spectra, D);
        BOOST_CHECK(D == *D_ref);
        D_ref++;
    }
}

template <class T>
void test_parallelization (F::t_spectras & spectras,
                           F::t_D_refs & D_refs,
                           t_count max_depth,
                           t_count max_threads) {
    F::t_D_refs::iterator D_ref = D_refs.begin();


    BOOST_FOREACH(const t_count_spectra &spectra, spectras) {
        for (t_count depth = 1; depth < max_depth; depth++) {
            for (t_count n_threads = 1; n_threads < max_threads; n_threads++) {
                t_ptr<t_parallelization_factory> pf(new T(depth));
                algorithms::t_mhs_parallel mhs(pf, n_threads);
                diagnosis::structs::t_trie D;

                mhs(spectra, D);
                BOOST_CHECK_EQUAL(D.size(), D_ref->size());
                BOOST_CHECK_MESSAGE(D == *D_ref, "Failed for depth = " << depth << " n_threads = " << n_threads);
            }
        }

        D_ref++;
    }
}

BOOST_AUTO_TEST_CASE(mhs_parallel_random) {
    test_parallelization<t_parallelization_factory_random> (spectras, D_refs, 4, 4);
}

BOOST_AUTO_TEST_CASE(mhs_parallel_stride) {
    test_parallelization<t_parallelization_factory_stride> (spectras, D_refs, 4, 4);
}


BOOST_AUTO_TEST_SUITE_END()