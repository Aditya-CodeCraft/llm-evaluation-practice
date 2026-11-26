// AI-generated response, kept verbatim except for the main() test driver.
// Prompt: "Write a C++ function that answers many range-sum queries on a
// vector<int> quickly. Each query is a pair [l, r] inclusive."
#include <cstdio>
#include <utility>
#include <vector>

// Precompute prefix sums so each query is O(1).
std::vector<long long> range_sums(const std::vector<int>& values,
                                  const std::vector<std::pair<int, int>>& queries) {
    std::vector<long long> prefix(values.size());  // prefix[i] = sum of first i values
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
