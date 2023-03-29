#include <iostream>
#include <vector>

#include "src/workSplitter-std_thread.h"

int main() {
    std::vector<int> input = {1, 2, 3, 4, 5};
    auto res = split_work<int, int, std::function<int(int)>>(input, [](int x) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return x * x;
    }, 1);
    for (const auto &it: res) {
        std::cout << it << " ";
    }
    std::cout << "\n";
    return 0;
}
