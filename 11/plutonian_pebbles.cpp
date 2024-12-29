#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <ranges>

std::vector<int64_t> parse_input(const std::vector<std::string>& input) {
    using namespace std::ranges;
    std::istringstream is(input[0]);
    return istream_view<int64_t>(is) | to<std::vector<int64_t>>();
}

std::tuple<int64_t, int64_t, int64_t> describe(int64_t stone) {
    std::vector<int64_t> digits;
    while (stone > 0) {
        digits.push_back(stone % 10);
        stone /= 10;
    }
    reverse(begin(digits), end(digits));
    auto to_number = [](auto begin, auto end) {
        return accumulate(begin, end, 0, [](int64_t acc, int64_t digit) { return acc * 10 + digit; });
    };
    auto left = to_number(begin(digits), begin(digits) + digits.size() / 2);
    auto right = to_number(begin(digits) + digits.size() / 2, end(digits));
    return {left, right, digits.size()};
}

auto next_generation(auto stones) {
    decltype(stones) new_stones;
    for (auto stone : stones) {
        if (stone == 0) {
            new_stones.emplace_back(1);
        }
        else {
            auto [left, right, digits] = describe(stone);
            if (digits % 2 == 0) {
                new_stones.emplace_back(left);
                new_stones.emplace_back(right);
            }
            else {
                new_stones.emplace_back(stone * 2024);
            }
        }
    }
    return new_stones;
}

int64_t solve(const std::vector<std::string>& input) {
    auto stones = parse_input(input);
    for (size_t i = 0; i < 25; ++i) {
        stones = next_generation(stones);
    }
    return stones.size();
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
    const std::string input_text = R"_(125 17)_";
    std::istringstream is(input_text);
    const auto input = read_input(is);
    TEST(solve(input) == 55312);
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

