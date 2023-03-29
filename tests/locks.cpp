#define BOOST_TEST_MODULE LOCKS

#include <boost/test/unit_test.hpp>
#include <semaphore.h>

#include "workSplitter-std_thread.h"

int slow(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return x;
}

std::vector<int> sample_int = {1, 2, 3};
std::vector<int> expected_int_without_mutex = {1, 2, 666};
std::vector<int> expected_int_with_mutex = {1, 2, 3};

BOOST_AUTO_TEST_CASE(without_locks) {
    auto spy = []() {
        sample_int.back() = 666;
    };
    std::vector<int> result;
    auto work = [&result]() {
        result = split_work<int, int>(sample_int, slow, 1);
    };
    std::thread worker = std::thread(work);
    std::thread spyer = std::thread(spy);
    spyer.join();
    worker.join();
    BOOST_ASSERT(result == expected_int_without_mutex);
}

BOOST_AUTO_TEST_CASE(with_locks) {
    auto spy = []() {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        sample_int.back() = 666;
    };
    std::vector<int> result;
    auto work = [&result]() {
        std::mutex mutex;
        result = split_work<int, int>(sample_int, slow, 1, mutex);
    };
    std::thread worker = std::thread(work);
    std::thread spyer = std::thread(spy);
    spyer.join();
    worker.join();
    BOOST_ASSERT(result == expected_int_with_mutex);
}
