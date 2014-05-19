#include <boost/test/unit_test.hpp>
#include "structs/filter.h"


#include <boost/foreach.hpp>
#include <boost/random/uniform_int_distribution.hpp>

using namespace diagnosis;
using namespace diagnosis::structs;


void randomize (t_filter & f, t_count count, t_count min, t_count max) {
    std::mt19937 gen;
    boost::random::uniform_int_distribution<t_count> rand(min, max);


    for (t_id i = 0; i < count; i++) {
        t_id id = rand(gen);

        if (f.is_filtered(id))
            i--;
        else
            f.filter(id);
    }

    BOOST_CHECK(f.get_filtered_count() == count);
}

BOOST_AUTO_TEST_SUITE(FILTER)
BOOST_AUTO_TEST_CASE(resize) {
    t_filter filter;


    BOOST_CHECK(filter.size() == 0);
    BOOST_CHECK(filter.get_filtered_count() == 0);

    filter.resize(1000);

    BOOST_CHECK(filter.size() == 1000);
    BOOST_CHECK(filter.get_filtered_count() == 0);

    for (t_id i = 1; i <= 1000; i++)
        BOOST_CHECK(!filter.is_filtered(i));

    BOOST_CHECK(filter.next_filtered(0) == 0);

    for (t_id i = 1; i <= 1000; i++)
        BOOST_CHECK(filter.next(i) == i + 1);
}

BOOST_AUTO_TEST_CASE(filter_id) {
    std::mt19937 gen;
    boost::random::uniform_int_distribution<t_count> rand(0, 10000);

    t_filter f1, f2;


    for (t_id i = 0; i < 100; i++) {
        t_id id = rand(gen);
        t_count max_size = std::max(f1.size(), id);
        f1.filter(id);

        BOOST_CHECK(f1.size() == max_size);

        for (t_id j = 1; j <= max_size; j++) {
            if (j == id)
                BOOST_CHECK(f1.is_filtered(id));
            else
                BOOST_CHECK(f1.is_filtered(j) == f2.is_filtered(j));
        }

        f2 = f1;
    }
}

BOOST_AUTO_TEST_CASE(unfilter_id) {
    std::mt19937 gen;
    boost::random::uniform_int_distribution<t_count> rand(0, 10000);
    t_filter f1, f2;


    randomize(f1, 1000, 1, 10001);

    for (t_id i = 0; i < 100; i++) {
        t_id id = rand(gen);

        if (!f1.is_filtered(id)) {
            i--;
            continue;
        }

        f2 = f1;
        f1.unfilter(id);

        BOOST_CHECK(f1.get_filtered_count() == 1000 - i - 1);

        for (t_id j = 1; j <= f1.size(); j++) {
            if (j == id)
                BOOST_CHECK(!f1.is_filtered(id));
            else
                BOOST_CHECK(f1.is_filtered(j) == f2.is_filtered(j));
        }
    }
}


BOOST_AUTO_TEST_CASE(next) {
    t_filter f1;


    randomize(f1, 1000, 1, 10001);


    BOOST_CHECK(f1.get_filtered_count() == 1000);

    t_id next = 0;
    t_count unfiltered_count = 0;

    while ((next = f1.next(next)) && next < f1.size()) {
        unfiltered_count++;
    }

    BOOST_CHECK(f1.size() == unfiltered_count + f1.get_filtered_count());
}

BOOST_AUTO_TEST_CASE(next_filtered) {
    t_filter f1;


    randomize(f1, 1000, 1, 10001);


    BOOST_CHECK(f1.get_filtered_count() == 1000);

    t_id next = 0;
    t_count filtered_count = 0;

    while ((next = f1.next_filtered(next))) {
        filtered_count++;
    }

    BOOST_CHECK(filtered_count == f1.get_filtered_count());
}

BOOST_AUTO_TEST_CASE(filter_all) {
    std::mt19937 gen;
    boost::random::uniform_int_distribution<t_count> rand(0, 10000);

    t_filter f1;
    t_candidate d;


    f1.filter_all(d);

    for (t_count i = 1; i <= f1.size(); i++)
        BOOST_CHECK(!f1.is_filtered(i));

    for (t_count i = 0; i < 200; i++) {
        d.insert(rand(gen));

        if (d.size() != i + 1)
            i--;
    }

    BOOST_CHECK(d.size() == 200);

    f1.filter_all(d);

    for (t_count i = 1; i <= f1.size(); i++)
        BOOST_CHECK((bool) d.count(i) == f1.is_filtered(i));
}

BOOST_AUTO_TEST_CASE(filter_all_but) {
    std::mt19937 gen;
    boost::random::uniform_int_distribution<t_count> rand(0, 10000);

    t_filter f1;
    t_candidate d;


    f1.filter_all_but(d);

    for (t_count i = 1; i <= f1.size(); i++)
        BOOST_CHECK(f1.is_filtered(i));

    f1 = t_filter();

    for (t_count i = 0; i < 200; i++) {
        d.insert(rand(gen));

        if (d.size() != i + 1)
            i--;
    }

    BOOST_CHECK(d.size() == 200);

    f1.filter_all_but(d);

    for (t_count i = 1; i <= f1.size(); i++)
        BOOST_CHECK((bool) d.count(i) == !f1.is_filtered(i));
}


BOOST_AUTO_TEST_CASE(filter_filter) {
    t_filter f1, f2, f3;


    randomize(f1, 1000, 1, 10001);
    randomize(f2, 1000, 1, 10001);

    f3 = f2;

    f3.filter(f1);

    for (t_count i = 1; i <= f3.size(); i++)
        BOOST_CHECK(f3.is_filtered(i) == (f1.is_filtered(i) || f2.is_filtered(i)));
}

BOOST_AUTO_TEST_CASE(unfilter_filter) {
    t_filter f1, f2, f3;


    randomize(f1, 1000, 1, 2001);
    randomize(f2, 1000, 1, 2001);

    f3 = f2;

    f3.unfilter(f1);

    for (t_count i = 1; i <= f3.size(); i++)
        BOOST_CHECK(f3.is_filtered(i) == (!f1.is_filtered(i) && f2.is_filtered(i)));
}

BOOST_AUTO_TEST_CASE(filter_iterator_no_filter) {
    t_filter_iterator it(100);


    t_id id = 0;


    while (it.next())
        BOOST_CHECK(++id == it.get());

    BOOST_CHECK(id == 100);
    BOOST_CHECK(it.get() == 0);

    for (id = 0; id < 100; id++) {
        it.set(id);

        while (it.next())
            BOOST_CHECK(++id == it.get());

        BOOST_CHECK(id == 100);
        BOOST_CHECK(it.get() == 0);
    }
}

BOOST_AUTO_TEST_CASE(filter_iterator) {
    t_filter f1;


    randomize(f1, 25, 1, 99);

    t_filter_iterator it(100, &f1);


    t_id id = 0;

    while (it.next()) {
        BOOST_CHECK(!f1.is_filtered(it.get()));

        while (++id != it.get())
            BOOST_CHECK(f1.is_filtered(id));
    }

    BOOST_CHECK(id == 100);
    BOOST_CHECK(it.get() == 0);

    for (id = 0; id < 100; id++) {
        it.set(id);

        while (it.next()) {
            BOOST_CHECK(!f1.is_filtered(it.get()));

            while (++id != it.get())
                BOOST_CHECK(f1.is_filtered(id));
        }

        BOOST_CHECK(id == 100);
        BOOST_CHECK(it.get() == 0);
    }
}


BOOST_AUTO_TEST_SUITE_END()