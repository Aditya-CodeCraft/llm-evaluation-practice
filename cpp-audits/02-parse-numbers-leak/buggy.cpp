// AI-generated response, kept verbatim except for the main() test driver.
// Prompt: "Write a C++ function that parses a comma-separated list of integers
// like "4,8,15" and returns false if any item is not a valid integer."
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

bool parse_numbers(const char* text, std::vector<int>& out) {
    // Copy the input because strtok modifies the string it tokenizes.
    char* buffer = new char[std::strlen(text) + 1];
    std::strcpy(buffer, text);

    for (char* token = std::strtok(buffer, ","); token != nullptr; token = std::strtok(nullptr, ",")) {
        char* end = nullptr;
        long value = std::strtol(token, &end, 10);
        if (end == token || *end != '\0') {
            return false;  // not a number
        }
        out.push_back(static_cast<int>(value));
    }

    delete[] buffer;
    return true;
}

int main() {
    // Parse a few input lines, as a caller would. (With a single bad line the
    // leak is hidden: strtok's internal pointer still refers to the buffer.)
    std::vector<int> good;
    std::vector<int> bad;
    const bool ok_bad = parse_numbers("4,eight,15", bad);
    const bool ok_bad2 = parse_numbers("1,2,x", bad);
    const bool ok_good = parse_numbers("4,8,15,16,23,42", good);
    if (!ok_good || good.size() != 6 || ok_bad || ok_bad2) {
        std::puts("wrong result");
        return 1;
    }
    std::puts("parse results correct");
    return 0;
}
