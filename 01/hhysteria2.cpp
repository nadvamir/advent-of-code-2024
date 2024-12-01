#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <numeric>
#include <algorithm>
#include <unordered_map>

std::pair<std::vector<int64_t>, std::vector<int64_t>> read_input_vectors(const std::vector<std::string>& input) {
    std::vector<int64_t> left, right;
    left.reserve(input.size());
    right.reserve(input.size());
    for (const auto& line : input) {
        int64_t a, b;
        std::istringstream is(line);
        is >> a >> b;
        left.push_back(a);
        right.push_back(b);
    }
    return {left, right};
}

std::unordered_map<int64_t, int64_t> calculate_frequencies(const std::vector<int64_t>& values) {
    std::unordered_map<int64_t, int64_t> frequencies;
    for (const auto& value : values) {
        frequencies[value]++;
    }
    return frequencies;
}

int64_t solve(const std::vector<std::string>& input) {
    const auto [left, right] = read_input_vectors(input);
    auto frequencies = calculate_frequencies(right);

    return accumulate(begin(left), end(left), 0, [&](int64_t acc, int64_t value) {
        return acc + value * frequencies[value];
    });
}

//------------------------------------------------------------------------------
#define TEST(a) { std::cout << #a << " == " << (a) << std::endl; assert((a)); }

std::vector<std::string> read_input(std::istream& is) {
    std::vector<std::string> input;
    std::string line;
    while (getline(is, line)) {
        input.push_back(line);
    }
    return input;
}

void test() {
    const std::string input_text = R"_(3   4
4   3
2   5
1   3
3   9
3   3
)_";
    std::istringstream is(input_text);
    const auto input = read_input(is);
    TEST(solve(input) == 31);
}

void run() {
    const auto input = read_input(std::cin);
    std::cout << solve(input) << "\n";
}

int main() {
    test();
    run();
}

