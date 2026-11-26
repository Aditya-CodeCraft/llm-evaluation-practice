// Fixed version of buggy.cpp: the copy of the input is owned by std::string,
// so every return path releases it, and strtok (which is not thread-safe and
// silently skips empty items) is no longer needed.
#include <cerrno>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

bool parse_numbers(const char* text, std::vector<int>& out) {
    const std::string input(text);
    std::vector<int> parsed;  // copied to `out` only on success
    std::size_t start = 0;
    while (start <= input.size()) {
        std::size_t comma = input.find(',', start);
        if (comma == std::string::npos) comma = input.size();
        const std::string token = input.substr(start, comma - start);

        char* end = nullptr;
        errno = 0;
        const long value = std::strtol(token.c_str(), &end, 10);
        if (token.empty() || *end != '\0' || errno == ERANGE || value < INT_MIN || value > INT_MAX) {
            return false;  // nothing to clean up: std::string and std::vector free themselves
        }
        parsed.push_back(static_cast<int>(value));
        start = comma + 1;
    }
    out.insert(out.end(), parsed.begin(), parsed.end());
    return true;
}

int main() {
    std::vector<int> good;
    std::vector<int> bad;
    const bool ok_good = parse_numbers("4,8,15,16,23,42", good);
    const bool ok_bad = parse_numbers("4,eight,15", bad);
    // Edge cases the original also got wrong: an empty item and int overflow.
    const bool ok_empty_item = parse_numbers("4,,15", bad);
    const bool ok_overflow = parse_numbers("99999999999", bad);
    if (!ok_good || good.size() != 6 || ok_bad || ok_empty_item || ok_overflow || !bad.empty()) {
        std::puts("wrong result");
        return 1;
    }
    std::puts("parse results correct");
    return 0;
}
