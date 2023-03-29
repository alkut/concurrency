#ifndef SPLIT_WORK_WORK_SPLITTER_H
#define SPLIT_WORK_WORK_SPLITTER_H

#include <functional>
#include <cassert>
#include <thread>
#include <mutex>
#include <type_traits>

#include "one_thread_array_work.h"

template<class TInput, class TOutput, class TFunction = std::function<TOutput(TInput)>>
std::vector<TOutput> split_work(const std::vector<TInput> &input, const TFunction &function,
                                size_t threshold);

template<class TInput, class TOutput, class TFunction = std::function<TOutput(TInput)>>
std::vector<TOutput> split_work(const std::vector<TInput> &input, const TFunction &function,
                                size_t threshold, std::mutex &mutex);


template<class TInput, class TOutput, class TFunction>
std::vector<TOutput>
split_work(const std::vector<TInput> &input, const TFunction &function, const size_t threshold) {
    static_assert(std::is_constructible_v<TOutput>, "Output type must have default constructor");
    assert(threshold != 0);
    std::vector<TOutput> output(input.size());
    size_t number_of_threads = (input.size() + threshold - 1) / threshold;
    std::thread workers[number_of_threads];
    for (size_t i = 0; i < number_of_threads; ++i) {
        workers[i] = std::thread(make_work_one_thread<TInput, TOutput, TFunction>,
                                 std::cref(input), std::ref(output), std::cref(function),
                                 i * threshold, std::min((i + 1) * threshold, input.size()));
    }
    for (size_t i = 0; i < number_of_threads; ++i) {
        workers[i].join();
    }
    return output;
}

template<class TInput, class TOutput, class TFunction>
std::vector<TOutput>
split_work(const std::vector<TInput> &input, const TFunction &function, size_t threshold, std::mutex &mutex) {
    static_assert(std::is_constructible_v<TOutput>, "Output type must have default constructor");
    assert(threshold != 0);
    std::lock_guard<std::mutex> scope_lock(mutex);
    std::vector<TOutput> output(input.size());
    size_t number_of_threads = (input.size() + threshold - 1) / threshold;
    std::thread workers[number_of_threads];
    for (size_t i = 0; i < number_of_threads; ++i) {
        workers[i] = std::thread(make_work_one_thread<TInput, TOutput, TFunction>,
                                 std::cref(input), std::ref(output), std::cref(function),
                                 i * threshold, std::min((i + 1) * threshold, input.size()));
    }
    for (size_t i = 0; i < number_of_threads; ++i) {
        workers[i].join();
    }
    return output;
}

#endif //SPLIT_WORK_WORK_SPLITTER_H
