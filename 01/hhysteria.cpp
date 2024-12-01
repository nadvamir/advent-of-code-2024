#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <ranges>

std::pair<std::vector<int>, std::vector<int>> read_input_vectors(const std::vector<std::string>& input) {
    auto parse_line = [](const std::string& line) {
        int64_t a, b;
        std::istringstream is(line);
        is >> a >> b;
        return std::make_pair(a, b);
    };

    std::vector<int> left, right;
    left.reserve(input.size());
    right.reserve(input.size());
    for (const auto [a, b] : input | std::views::transform(parse_line)) {
        left.push_back(a);
        right.push_back(b);
    }
    return {left, right};
}

int solve(const std::vector<std::string>& input) {
    auto [left, right] = read_input_vectors(input);
    std::ranges::sort(left);
    std::ranges::sort(right);

    int result = 0;
    for (size_t i = 0; i < left.size(); ++i) {
        result += std::abs(left[i] - right[i]);
    }

    return result;
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
    TEST(solve(input) == 11);
}

void run() {
    const auto input = read_input(std::cin);

    using namespace std::chrono;
    const auto start_ts = high_resolution_clock::now();

    std::cout << solve(input) << "\n";
    
    const auto end_ts = high_resolution_clock::now();
    std::cout << "Elapsed time: " << duration_cast<microseconds>(end_ts - start_ts).count() << "us\n";
}

int main() {
    test();
    run();
}

