#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <map>
#include <utility>

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

int64_t evolve(int64_t stone, int64_t generations, auto& cache) {
    if (generations == 0) return 1;
    if (auto it = cache.find({stone, generations}); it != end(cache)) {
        return it->second;
    }
    int64_t result = 0;
    if (stone == 0) {
        result = evolve(1, generations - 1, cache);
    }
    else {
        auto [left, right, digits] = describe(stone);
        if (digits % 2 == 0) {
            result += evolve(left, generations - 1, cache);
            result += evolve(right, generations - 1, cache);
        }
        else {
            result = evolve(stone * 2024, generations - 1, cache);
        }
    }
    cache[{stone, generations}] = result;
    return result;
}

int64_t solve(const std::vector<std::string>& input, size_t N) {
    auto stones = parse_input(input);
    std::map<std::pair<int64_t, size_t>, int64_t> cache;
    return accumulate(begin(stones), end(stones), 0ll, [&](int64_t acc, int64_t stone) {
       return acc + evolve(stone, N, cache);
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
    const std::string input_text = R"_(125 17)_";
    std::istringstream is(input_text);
    const auto input = read_input(is);
    TEST(solve(input, 25) == 55312);
}

void run() {
    const auto input = read_input(std::cin);

    using namespace std::chrono;
    const auto start_ts = high_resolution_clock::now();

    std::cout << solve(input, 75) << "\n";
    
    const auto end_ts = high_resolution_clock::now();
    std::cout << "Elapsed time: " << duration_cast<microseconds>(end_ts - start_ts).count() << "us\n";
}

int main() {
    test();
    run();
}

