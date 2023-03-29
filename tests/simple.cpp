#define BOOST_TEST_MODULE PURE

#include <boost/test/unit_test.hpp>

#include "workSplitter-std_thread.h"

int square(int x) {
    return x * x;
}

struct functor {
    int operator() (int x) const {
        return x * x;
    }
};

std::vector<int> sample_int = {1, 2, 3};
std::vector<int> expected_int_without_mutex = {1, 4, 9};

BOOST_AUTO_TEST_CASE(c_function) {
    for (int threshold = 1; threshold < sample_int.size() + 2; ++threshold) {
        auto result = split_work<int, int>(sample_int, square, threshold);
        BOOST_ASSERT(result == expected_int_without_mutex);
    }
}

BOOST_AUTO_TEST_CASE(lambda) {
    for (int threshold = 1; threshold < sample_int.size() + 2; ++threshold) {
        auto result = split_work<int, int>(sample_int, [](int x) {
            return x * x;
            }, threshold);
        BOOST_ASSERT(result == expected_int_without_mutex);
    }
}

BOOST_AUTO_TEST_CASE(functor_class) {
    const functor sq;
    for (int threshold = 1; threshold < sample_int.size() + 2; ++threshold) {
        auto result = split_work<int, int>(sample_int, sq, threshold);
        BOOST_ASSERT(result == expected_int_without_mutex);
    }
}


