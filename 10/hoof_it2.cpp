#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
#include <ranges>
#include <set>
#include <utility>
#include <optional>

int64_t map_trails(const auto& input, auto i, auto j, auto& reachability) {
    if (reachability[i][j] != -1) return reachability[i][j];
    if (input[i][j] == '9') {
        reachability[i][j] = 1;
        return reachability[i][j];
    }

    auto out_of_bounds = [&input](auto i, auto j) { return i < 0 || j < 0 || i >= input.size() || j >= input[i].size(); };
    char next = input[i][j] + 1;
    reachability[i][j] = 0;

    constexpr std::array<std::pair<int, int>, 4> directions{{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};
    for (auto [di, dj] : directions) {
        auto ni = i + di, nj = j + dj;
        if (out_of_bounds(ni, nj)) continue;
        if (input[ni][nj] != next) continue;
        reachability[i][j] += map_trails(input, ni, nj, reachability);
    }
    return reachability[i][j];
}

int64_t solve(const std::vector<std::string>& input) {
    std::vector<std::vector<int64_t>> reachability(input.size(), std::vector<int64_t>(input[0].size(), -1));
    int64_t result = 0;
    for (int64_t i = 0; i < input.size(); ++i) {
        for (int64_t j = 0; j < input[i].size(); ++j) {
            if (input[i][j] == '0') {
                result += map_trails(input, i, j, reachability);
            }
        }
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
    const std::string input_text = R"_(89010123
78121874
87430965
96549874
45678903
32019012
01329801
10456732)_";
    std::istringstream is(input_text);
    const auto input = read_input(is);
    TEST(solve(input) == 81);
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

