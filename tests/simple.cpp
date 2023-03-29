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

std::vector<char> to_upper(const std::vector<char>& in) {
    std::vector<char> result;
    for (const auto& ch: in)
        result.push_back(std::toupper(ch));
    return result;
}

std::vector<int> sample_int = {1, 2, 3};
std::vector<int> expected_int = {1, 4, 9};

std::vector<std::vector<char>> sample_varchar = {
        {'a', 'c', 'v'},
        {'U', 'g'},
        {'p'}
};
std::vector<std::vector<char>> expected_varchar = {
        {'A', 'C', 'V'},
        {'U', 'G'},
        {'P'}
};

BOOST_AUTO_TEST_CASE(c_function) {
    for (int threshold = 1; threshold < sample_int.size() + 2; ++threshold) {
        auto result = split_work<int, int>(sample_int, square, threshold);
        BOOST_ASSERT(result == expected_int);
    }
}

BOOST_AUTO_TEST_CASE(lambda) {
    for (int threshold = 1; threshold < sample_int.size() + 2; ++threshold) {
        auto result = split_work<int, int>(sample_int, [](int x) {
            return x * x;
            }, threshold);
        BOOST_ASSERT(result == expected_int);
    }
}

BOOST_AUTO_TEST_CASE(functor_class) {
    const functor sq;
    for (int threshold = 1; threshold < sample_int.size() + 2; ++threshold) {
        auto result = split_work<int, int>(sample_int, sq, threshold);
        BOOST_ASSERT(result == expected_int);
    }
}

BOOST_AUTO_TEST_CASE(empty_input) {
    auto test = [](){
        auto result = split_work<int, int, std::function<int(int)>>({}, square, 5);
        BOOST_CHECK(result.empty());
    };
    BOOST_CHECK_NO_THROW(test());
}

BOOST_AUTO_TEST_CASE(non_trivial_type) {
    for (int threshold = 1; threshold < sample_varchar.size() + 2; ++threshold) {
        auto result = split_work<std::vector<char>, std::vector<char>>(sample_varchar, to_upper, threshold);
        BOOST_ASSERT(result == expected_varchar);
    }
}


