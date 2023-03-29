#ifndef SPLIT_WORK_WORK_SPLITTER_H
#define SPLIT_WORK_WORK_SPLITTER_H

#include <functional>
#include <cassert>
#include <thread>
#include <mutex>
#include <type_traits>

#include "one_thread_array_work.h"

/**
 *
 * @tparam TOutput must have default constructor
 * @param function must be pure and noexcept
 * @param threshold must not be equal to zero
 * @Note thread unsafe
 */
template<class TInput, class TOutput, class TFunction = std::function<TOutput(TInput)>>
std::vector<TOutput> split_work(const std::vector<TInput> &input, const TFunction &function,
                                size_t threshold);


/**
 *
 * @tparam TOutput must have default constructor
 * @param function must be pure and noexcept
 * @param threshold must not be equal to zero
 * @Note thread safe
 */
template<class TInput, class TOutput, class TFunction = std::function<TOutput(TInput)>>
std::vector<TOutput> split_work(const std::vector<TInput> &input, const TFunction &function,
                                size_t threshold, std::mutex &mutex);


template<class TInput, class TOutput, class TFunction = std::function<TOutput(TInput)>>
std::vector<TOutput> split_work_validation(const std::vector<TInput> &input, const TFunction &function,
                                     size_t threshold);


template<class TInput, class TOutput, class TFunction = std::function<TOutput(TInput)>>
std::vector<TOutput> split_work_impl(const std::vector<TInput> &input, const TFunction &function,
                                size_t threshold);






template<class TInput, class TOutput, class TFunction>
std::vector<TOutput>
split_work(const std::vector<TInput> &input, const TFunction &function, const size_t threshold) {
    const auto result = split_work_validation<TInput, TOutput, TFunction>(input, function, threshold);
    if (input.size() <= threshold) {
        return result;
    }
    return split_work_impl<TInput, TOutput, TFunction>(input, function, threshold);
}

template<class TInput, class TOutput, class TFunction>
std::vector<TOutput>
split_work(const std::vector<TInput> &input, const TFunction &function, size_t threshold, std::mutex &mutex) {
    const auto result = split_work_validation<TInput, TOutput, TFunction>(input, function, threshold);
    if (input.size() <= threshold) {
        return result;
    }
    std::lock_guard<std::mutex> scope_lock(mutex);
    return split_work_impl<TInput, TOutput, TFunction>(input, function, threshold);
}

template<class TInput, class TOutput, class TFunction>
std::vector<TOutput>
split_work_validation(const std::vector<TInput> &input, const TFunction &function, size_t threshold) {
    if (input.empty()) {
        return {};
    }
    static_assert(std::is_constructible_v<TOutput>, "Output type must have default constructor");
    assert(threshold != 0);
    std::vector<TOutput> output(input.size());
    if (input.size() <= threshold) {
        make_work_one_thread(input, output, function,
                             0, input.size());
        return output;
    }
    return {};
}


template<class TInput, class TOutput, class TFunction>
std::vector<TOutput> split_work_impl(const std::vector<TInput> &input, const TFunction &function, size_t threshold) {
    ///include current thread itself
    size_t number_of_threads = (input.size() + threshold - 1) / threshold;
    std::thread workers[number_of_threads - 1];
    std::vector<TOutput> output(input.size());
    for (size_t i = 0; i < number_of_threads - 1; ++i) {
        workers[i] = std::thread(make_work_one_thread<TInput, TOutput, TFunction>,
                                 std::cref(input), std::ref(output), std::cref(function),
                                 i * threshold, (i + 1) * threshold);
    }
    make_work_one_thread(input, output, function,
                         (number_of_threads - 1) * threshold, input.size());
    for (size_t i = 0; i < number_of_threads - 1; ++i) {
        workers[i].join();
    }
    return output;
}

#endif //SPLIT_WORK_WORK_SPLITTER_H
