#define BOOST_TEST_MODULE PURE

#include <boost/test/unit_test.hpp>

#include "workSplitter-std_thread.h"

std::vector<int> sample_int = {0, 0, 0};
std::vector<int> expected_int = {0, 0, 1};

int dirty_function(int x) {
    ++sample_int.back();
    return x;
}

//when function provided isn't a pure function, behaviour became undefined - this test can randomly fail
//(if 3rd thread return value before other write into sample_int)
BOOST_AUTO_TEST_CASE(dirty) {
    auto result = split_work<int, int>(sample_int, dirty_function, 1);
    BOOST_ASSERT(result != expected_int);
}