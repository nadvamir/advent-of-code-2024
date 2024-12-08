#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <ranges>

std::vector<std::vector<int>> parse_input(const std::vector<std::string>& input) {
    using namespace std::ranges;
    auto parse_line = [](const std::string& line) {
        std::istringstream is(line);
        return istream_view<int>(is) | to<std::vector<int>>();
    };

    return input | std::views::transform(parse_line) | to<std::vector<std::vector<int>>>();
}

int64_t solve(const std::vector<std::string>& input) {
    using namespace std::ranges;

    auto reports = parse_input(input);

    auto is_safe_sequence = [](const auto& report) {
        if (report.size() < 2) return true;
        int prev_diff = report[1] - report[0];
        for (auto prev = begin(report), it = next(prev); it != end(report); ++prev, ++it) {
            int diff = *it - *prev;
            if (diff == 0 || abs(diff) > 3) return false;
            if (prev_diff * diff <= 0) return false;
            prev_diff = diff;
        }
        return true;
    };

    return count_if(reports, [&](const auto& report) {
        for (size_t i = 0; i < report.size(); ++i) {
            auto report_copy = report;
            report_copy.erase(begin(report_copy) + i);
            if (is_safe_sequence(report_copy)) {
                return true;
            }
        }
        return false;
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
    const std::string input_text = R"_(7 6 4 2 1
1 2 7 8 9
9 7 6 2 1
1 3 2 4 5
8 6 4 4 1
1 3 6 7 9)_";
    std::istringstream is(input_text);
    const auto input = read_input(is);
    TEST(solve(input) == 4);
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

