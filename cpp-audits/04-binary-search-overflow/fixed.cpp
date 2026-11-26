// Fixed version of buggy.cpp: the midpoint is computed without overflow.
#include <cstdio>
#include <functional>

int first_true(int lo, int hi, const std::function<bool(int)>& ok) {
    while (lo < hi) {
        // With 0 <= lo < hi, hi - lo cannot overflow and lo + (hi - lo) / 2
        // stays in [lo, hi). (If lo may be negative, hi - lo can still overflow;
        // C++20's std::midpoint handles every case.)
        int mid = lo + (hi - lo) / 2;
        if (ok(mid)) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

int main() {
    const int threshold = 2000000000;
    const int got = first_true(0, 2147483647, [threshold](int x) { return x >= threshold; });
    std::printf("expected %d, got %d\n", threshold, got);
    return got == threshold ? 0 : 1;
}
