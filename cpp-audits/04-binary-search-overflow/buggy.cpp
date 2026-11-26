// AI-generated response, kept verbatim except for the main() test driver.
// Prompt: "Write a C++ function that finds the smallest x in [lo, hi] for which
// a monotonic predicate ok(x) is true (binary search on the answer)."
#include <cstdio>
#include <functional>

int first_true(int lo, int hi, const std::function<bool(int)>& ok) {
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (ok(mid)) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

int main() {
    // Smallest x with x >= 2000000000, searched over the full non-negative int range.
    const int threshold = 2000000000;
    const int got = first_true(0, 2147483647, [threshold](int x) { return x >= threshold; });
    std::printf("expected %d, got %d\n", threshold, got);
    return got == threshold ? 0 : 1;
}
