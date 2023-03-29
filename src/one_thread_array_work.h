#ifndef SPLIT_WORK_ONE_THREAD_ARRAY_WORK_H
#define SPLIT_WORK_ONE_THREAD_ARRAY_WORK_H

#include <cassert>
#include <vector>

template<class TInput, class TOutput, class TFunction>
void make_work_one_thread(const std::vector<TInput> &input, std::vector<TOutput>& output,
                          const TFunction &function, size_t start_idx, size_t end_idx) noexcept;

template<class TInput, class TOutput, class TFunction>
void make_work_one_thread(const std::vector<TInput> &input, std::vector<TOutput>& output,
                          const TFunction &function, size_t start_idx, size_t end_idx) noexcept {
    try {
        assert(end_idx > start_idx);
        for (size_t i = start_idx; i < end_idx; ++i) {
            output[i] = function(input[i]);
        }
    } catch (const std::exception& exception) {
        std::cerr << "something happened during one thread processing: " <<  exception.what();
    }
}

#endif //SPLIT_WORK_ONE_THREAD_ARRAY_WORK_H
