#include <boost/test/unit_test.hpp>
#include "diagnosis/algorithms/mhs.h"
#include "diagnosis/heuristics/sort.h"
#include "diagnosis/heuristics/similarity.h"
#include "diagnosis/heuristics/parallelization.h"
#include "diagnosis/structs/count_spectra.h"
#include "diagnosis/benchmark/generators/bernoulli.h"

#include <fstream>
using namespace diagnosis;
using namespace diagnosis::algorithms;
using namespace diagnosis::heuristics;
using namespace diagnosis::structs;
using namespace diagnosis::benchmark;

BOOST_AUTO_TEST_SUITE(MHS2)

BOOST_AUTO_TEST_CASE(stop_flags) {
    t_spectra * spectra;
    t_candidate correct;
    t_bernoulli randomizer(0.25, 1, 100, 100);
    std::mt19937 gen;
    t_trie D;


    spectra = randomizer(gen, correct);

    t_mhs mhs;
    mhs.max_candidate_size = 2;
    mhs(*spectra, D);

    // Max Candidate Size
    t_trie::iterator it = D.begin();

    while (it != D.end())
        BOOST_CHECK((it++)->size() <= mhs.max_candidate_size);

    // Max Candidate Size
    mhs = t_mhs();
    mhs.max_candidates = 4000;
    D.clear();
    mhs(*spectra, D);
    BOOST_CHECK(D.size() <= mhs.max_candidates);

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

BOOST_AUTO_TEST_CASE(parallelization) {
    t_spectra * spectra;
    t_candidate correct;
    t_bernoulli randomizer(0.25, 1, 25, 22);
    std::mt19937 gen;
    t_trie reference;


    spectra = randomizer(gen, correct);

    {
        t_mhs mhs;
        reference.clear();
        mhs(*spectra, reference);
    }

    t_count level = 2;

    for (t_count stride = 0; stride < 4; stride++) {
        for (t_count ntasks = 1; ntasks < 10; ntasks++) {
            t_trie D;
            D.clear();

            for (t_count rank = 0; rank < ntasks; rank++) {
                t_mhs mhs;


                mhs.set_heuristic(level + 1, mhs.get_heuristic_ptr(level));

                t_ptr<t_heuristic> heuristic(new t_heuristic(mhs.get_heuristic(level)));

                if (stride)
                    heuristic->push(new heuristics::t_divide(rank, ntasks, stride));
                else
                    heuristic->push(new heuristics::t_random_divide(rank, ntasks, 1234));

                mhs.set_heuristic(level, heuristic);

                BOOST_CHECK(mhs.get_heuristic(level - 1) == mhs.get_heuristic(level + 1));
                BOOST_CHECK(mhs.get_heuristic(level - 1) != mhs.get_heuristic(level));
                BOOST_CHECK(mhs.get_heuristic(level + 1) != mhs.get_heuristic(level));
                // std::cout << mhs.get_heuristic(level - 1) << mhs.get_heuristic(level) << mhs.get_heuristic(level + 1) << std::endl;

                mhs(*spectra, D);
            }

            BOOST_CHECK_MESSAGE(D == reference, "Failed for stride = " << stride << " ntasks = " << ntasks <<
                                ". D.size() = " << D.size() <<
                                " reference.size() = " << reference.size());
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()