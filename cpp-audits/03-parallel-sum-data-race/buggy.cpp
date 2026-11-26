// AI-generated response, kept verbatim except for the main() test driver.
// Prompt: "Speed up summing a large vector<int> in C++ by using 4 threads."
#include <cstdio>
#include <numeric>
#include <thread>
#include <vector>

long long parallel_sum(const std::vector<int>& data, unsigned thread_count = 4) {
    long long total = 0;
    std::vector<std::thread> threads;
    const std::size_t chunk = data.size() / thread_count;

    for (unsigned t = 0; t < thread_count; ++t) {
        const std::size_t begin = t * chunk;
        const std::size_t end = (t == thread_count - 1) ? data.size() : begin + chunk;
        threads.emplace_back([&, begin, end] {
            for (std::size_t i = begin; i < end; ++i) {
                total += data[i];  // each thread adds its part to the total
            }
        });
    }
    for (auto& th : threads) th.join();
    return total;
}

int main() {
    std::vector<int> data(1000000);
    std::iota(data.begin(), data.end(), 1);
    const long long expected = 1000000LL * 1000001LL / 2;
    const long long got = parallel_sum(data);
    std::printf("expected %lld, got %lld\n", expected, got);
    return got == expected ? 0 : 1;
}
