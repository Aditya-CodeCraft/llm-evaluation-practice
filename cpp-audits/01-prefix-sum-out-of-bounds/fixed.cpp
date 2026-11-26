// Fixed version of buggy.cpp. The only change: prefix has n + 1 slots.
#include <cstdio>
#include <utility>
#include <vector>

std::vector<long long> range_sums(const std::vector<int>& values,
                                  const std::vector<std::pair<int, int>>& queries) {
    // prefix[i] = sum of the first i values for i = 0..n, so it needs n + 1 slots.
    std::vector<long long> prefix(values.size() + 1, 0);
    for (std::size_t i = 0; i < values.size(); ++i) {
        prefix[i + 1] = prefix[i] + values[i];
    }
    std::vector<long long> answers;
    answers.reserve(queries.size());
    for (const auto& [l, r] : queries) {
        answers.push_back(prefix[r + 1] - prefix[l]);
    }
    return answers;
}

int main() {
    const std::vector<int> values = {3, 1, 4, 1, 5, 9, 2, 6};
    const auto answers = range_sums(values, {{0, 7}, {2, 4}, {5, 5}});
    const std::vector<long long> expected = {31, 10, 9};
    if (answers != expected) {
        std::puts("wrong answer");
        return 1;
    }
    std::puts("all range sums correct");
    return 0;
}
