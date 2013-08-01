#include <boost/test/unit_test.hpp>
#include "diagnosis/spectra/count_spectra.h"
#include "diagnosis/algorithms/mhs.h"
#include "diagnosis/heuristics/sort.h"
#include "diagnosis/heuristics/similarity.h"

#include <fstream>
using namespace diagnosis;

BOOST_AUTO_TEST_SUITE(MHS2)

BOOST_AUTO_TEST_CASE(mhs) {
    for (t_id i = 0; i < 30; i++) {
        t_count_spectra spectra;
        diagnosis::structs::t_trie D, D_ref;

        std::stringstream s;
        std::ifstream input;

        s << "io/mhs/in." << i << ".txt";
        input.open(s.str().c_str());
        input >> spectra;

        input.close();
        s.str("");

        s << "io/mhs/out." << i << ".txt";
        input.open(s.str().c_str());
        input >> D_ref;

        t_heuristic heuristic;
        heuristic.push(new heuristics::t_ochiai());
        heuristic.push(new heuristics::t_sort());

        algorithms::t_mhs mhs(heuristic);

        mhs.calculate(spectra, D);
        std::cout << D.size() << " ";
        std::cout << D_ref.size() << std::endl;
        BOOST_CHECK(D == D_ref);
    }
}

BOOST_AUTO_TEST_SUITE_END()