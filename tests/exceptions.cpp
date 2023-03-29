#define BOOST_TEST_MODULE EXCEPTIONS

#include <boost/test/unit_test.hpp>

#include "workSplitter-std_thread.h"

std::vector<int> sample_int = {2, 3, 4};
std::vector<int> expected_int_without_mutex = {4, 9, 0};

int raise_exception(int x) {
    if (x == 4) {
        throw std::invalid_argument("i don't like 4");
    }
    return x * x;
}

//if exception would be thrown by function, split_work handle it, but output contains default value
BOOST_AUTO_TEST_CASE(exceptions) {
    auto result = split_work<int, int>(sample_int, raise_exception, 1);
    BOOST_ASSERT(result == expected_int_without_mutex);
}
