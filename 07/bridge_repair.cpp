#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <ranges>

auto parse_input(const std::vector<std::string>& input) {
    using namespace std::ranges;
    auto parse_line = [](const std::string& line) {
        std::istringstream is(line);
        int64_t sum; char c;
        is >> sum >> c;
        return std::make_pair(sum, istream_view<int64_t>(is) | to<std::vector<int64_t>>());
    };

    using ResultT = std::vector<std::pair<int64_t, std::vector<int64_t>>>;
    return input | std::views::transform(parse_line) | to<ResultT>();
}

auto achievable(int64_t target, const auto& operands, size_t idx, int64_t acc) {
    if (idx == operands.size()) return target == acc;
    if (target < 0) return false;
    if (achievable(target, operands, idx + 1, acc + operands[idx])) return true;
    if (achievable(target, operands, idx + 1, acc * operands[idx])) return true;
    return false;
}

int64_t solve(const std::vector<std::string>& input) {
    auto equations = parse_input(input);
    int64_t sum = 0;
    for (const auto& [target, operands] : equations) {
        if (achievable(target, operands, 0, 0)) {
            sum += target;
        }
    }
    return sum;
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
    const std::string input_text = R"_(190: 10 19
3267: 81 40 27
83: 17 5
156: 15 6
7290: 6 8 6 15
161011: 16 10 13
192: 17 8 14
21037: 9 7 18 13
292: 11 6 16 20)_";
    std::istringstream is(input_text);
    const auto input = read_input(is);
    TEST(solve(input) == 3749);
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

