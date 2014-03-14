#include <boost/test/unit_test.hpp>
#include "diagnosis/algorithms/mhs.h"
// #include "diagnosis/algorithms/mhs/sort.h"
// #include "diagnosis/algorithms/mhs/similarity.h"
// #include "diagnosis/algorithms/mhs/parallelization.h"
#include "diagnosis/structs/count_spectra.h"
#include "diagnosis/benchmark/generators/bernoulli.h"

#include <fstream>
using namespace diagnosis;
using namespace diagnosis::algorithms;
using namespace diagnosis::structs;
using namespace diagnosis::benchmark;

BOOST_AUTO_TEST_SUITE(MHS2)
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
    for (t_id i = 0; i < 30; i++) {
        t_count_spectra spectra;
        diagnosis::structs::t_trie D, D_ref;

        std::stringstream s;
        std::ifstream f;

        // Reading Input
        s << "io/mhs/in." << i << ".txt";
        f.open(s.str().c_str());
        f >> spectra;
        BOOST_CHECK(f.good());

        f.close();
        s.str("");

        // Reading Output
        s << "io/mhs/out." << i << ".txt";
        f.open(s.str().c_str());
        f >> D_ref;
        BOOST_CHECK(f.good());
        f.close();

        algorithms::t_mhs mhs;

        mhs(spectra, D);
        BOOST_CHECK(D == D_ref);

        t_trie::iterator it = D.begin();

        while (it != D.end())
            BOOST_CHECK(spectra.is_candidate(*(it++)));
    }
}

BOOST_AUTO_TEST_CASE(mhs_parallel_random) {
    for (t_id i = 0; i < 30; i++) {
        t_count_spectra spectra;
        diagnosis::structs::t_trie D_ref;

        std::stringstream s;
        std::ifstream f;

        // Reading Input
        s << "io/mhs/in." << i << ".txt";
        f.open(s.str().c_str());
        f >> spectra;
        BOOST_CHECK(f.good());

        f.close();
        s.str("");

        // Reading Output
        s << "io/mhs/out." << i << ".txt";
        f.open(s.str().c_str());
        f >> D_ref;
        BOOST_CHECK(f.good());
        f.close();

        for (t_count depth = 1; depth < 10; depth++) {
            for (t_count n_threads = 2; n_threads < 10; n_threads++) {
                diagnosis::structs::t_trie D;
                t_ptr<t_parallelization_factory> pf(new t_parallelization_factory_random(depth, time_interval()));

                algorithms::t_mhs_parallel mhs(pf, n_threads);

                mhs(spectra, D);
                BOOST_CHECK_EQUAL(D.size(), D_ref.size());
                BOOST_CHECK_MESSAGE(D == D_ref, "Failed for depth = " << depth << " n_threads = " << n_threads);

                t_trie::iterator it = D.begin();

                while (it != D.end())
                    BOOST_CHECK(spectra.is_candidate(*(it++)));
            }
        }
    }
}


BOOST_AUTO_TEST_CASE(mhs_parallel_stride) {
    for (t_id i = 0; i < 30; i++) {
        t_count_spectra spectra;
        diagnosis::structs::t_trie D_ref;

        std::stringstream s;
        std::ifstream f;

        // Reading Input
        s << "io/mhs/in." << i << ".txt";
        f.open(s.str().c_str());
        f >> spectra;
        BOOST_CHECK(f.good());

        f.close();
        s.str("");

        // Reading Output
        s << "io/mhs/out." << i << ".txt";
        f.open(s.str().c_str());
        f >> D_ref;
        BOOST_CHECK(f.good());
        f.close();

        for (t_count depth = 1; depth < 10; depth++) {
            for (t_count n_threads = 1; n_threads < 10; n_threads++) {
                diagnosis::structs::t_trie D;
                t_ptr<t_parallelization_factory> pf(new t_parallelization_factory_stride(depth));

                algorithms::t_mhs_parallel mhs(pf, n_threads);

                mhs(spectra, D);
                BOOST_CHECK_EQUAL(D.size(), D_ref.size());
                BOOST_CHECK_MESSAGE(D == D_ref, "Failed for depth = " << depth << " n_threads = " << n_threads);

                t_trie::iterator it = D.begin();

                while (it != D.end())
                    BOOST_CHECK(spectra.is_candidate(*(it++)));
            }
        }
    }
}


BOOST_AUTO_TEST_CASE(parallelization_stride) {
    t_spectra * spectra;
    t_candidate correct;
    t_bernoulli randomizer(0.25, 1, 25, 22);
    std::mt19937 gen;
    t_trie D_ref;


    spectra = randomizer(gen, correct);

    {
        t_mhs mhs;
        D_ref.clear();
        mhs(*spectra, D_ref);
    }

    for (t_count depth = 1; depth < 10; depth++) {
        for (t_count n_threads = 1; n_threads < 10; n_threads++) {
            t_trie D;
            D.clear();

            for (t_count rank = 0; rank < n_threads; rank++) {
                t_mhs mhs;
                t_ptr<t_parallelization> parallelization(new t_parallelization_stride(rank, n_threads, depth));

                mhs.set_parallelization(parallelization);
                mhs(*spectra, D);
            }

            BOOST_CHECK_EQUAL(D.size(), D_ref.size());
            BOOST_CHECK_MESSAGE(D == D_ref, "Failed for depth = " << depth << " n_threads = " << n_threads);
        }
    }
}

BOOST_AUTO_TEST_CASE(parallelization_random) {
    t_spectra * spectra;
    t_candidate correct;
    t_bernoulli randomizer(0.25, 1, 25, 22);
    std::mt19937 gen;
    t_trie D_ref;


    spectra = randomizer(gen, correct);

    {
        t_mhs mhs;
        D_ref.clear();
        mhs(*spectra, D_ref);
    }

    for (t_count depth = 1; depth < 10; depth++) {
        for (t_count n_threads = 1; n_threads < 10; n_threads++) {
            t_trie D;
            D.clear();

            for (t_count rank = 0; rank < n_threads; rank++) {
                t_mhs mhs;
                t_ptr<t_parallelization> parallelization(new t_parallelization_random(rank, n_threads, depth, 1234));

                mhs.set_parallelization(parallelization);
                mhs(*spectra, D);
            }

            BOOST_CHECK_EQUAL(D.size(), D_ref.size());
            BOOST_CHECK_MESSAGE(D == D_ref, "Failed for depth = " << depth << " n_threads = " << n_threads);
        }
    }
}
BOOST_AUTO_TEST_SUITE_END()