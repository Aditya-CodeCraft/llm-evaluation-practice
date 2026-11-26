// Fixed version of buggy.cpp: each thread sums into a local variable and
// publishes it to its own slot; the slots are combined after join(), so no
// memory location is written by two threads.
#include <cstdio>
#include <numeric>
#include <thread>
#include <vector>

long long parallel_sum(const std::vector<int>& data, unsigned thread_count = 4) {
    if (thread_count == 0) thread_count = 1;
    std::vector<long long> partial(thread_count, 0);
    std::vector<std::thread> threads;
    const std::size_t chunk = data.size() / thread_count;

    for (unsigned t = 0; t < thread_count; ++t) {
        const std::size_t begin = t * chunk;
        const std::size_t end = (t == thread_count - 1) ? data.size() : begin + chunk;
        threads.emplace_back([&data, &partial, t, begin, end] {
            long long local = 0;
            for (std::size_t i = begin; i < end; ++i) local += data[i];
            partial[t] = local;  // a distinct element per thread: no race
        });
    }
    for (auto& th : threads) th.join();  // join() makes each thread's write visible here
    return std::accumulate(partial.begin(), partial.end(), 0LL);
}

int main() {
    std::vector<int> data(1000000);
    std::iota(data.begin(), data.end(), 1);
    const long long expected = 1000000LL * 1000001LL / 2;
    const long long got = parallel_sum(data);
    std::printf("expected %lld, got %lld\n", expected, got);
    return got == expected ? 0 : 1;
}
