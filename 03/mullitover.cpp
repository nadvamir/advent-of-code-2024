#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <regex>

int64_t solve(const std::vector<std::string>& input) {
    const auto joined = accumulate(begin(input), end(input), std::string());
    const auto pattern = std::regex(R"_(mul\((\d{1,3}),(\d{1,3})\))_");

    std::sregex_iterator begin(joined.begin(), joined.end(), pattern);
    std::sregex_iterator end;
    int64_t result = 0;
    for (auto it = begin; it != end; ++it) {
        const int first = std::stoi((*it)[1].str());
        const int second = std::stoi((*it)[2].str());
        result += first * second;
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
    const std::string input_text = R"_(xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5)))_";
    std::istringstream is(input_text);
    const auto input = read_input(is);
    TEST(solve(input) == 161);
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

