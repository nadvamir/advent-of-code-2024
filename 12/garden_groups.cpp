#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <ranges>
#include <set>
#include <utility>

std::pair<int64_t, int64_t> fence_cost(auto& input, int64_t i, int64_t j, char c, auto& visited) {
    visited.insert({i, j});
    int64_t area = 1, perimeter = 0;
    auto out_of_bounds = [&input](int64_t i, int64_t j) { return i < 0 || j < 0 || i >= input.size() || j >= input[i].size(); };
    constexpr std::array<std::pair<int, int>, 4> directions{{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};
    for (auto [di, dj] : directions) {
        auto ni = i + di, nj = j + dj;
        if (out_of_bounds(ni, nj)) {
            ++perimeter;
        }
        else if (input[ni][nj] != c) {
            ++perimeter;
        }
        else if (visited.contains({ni, nj})) {
            continue;
        }
        else {
            auto [a, p] = fence_cost(input, ni, nj, c, visited);
            area += a;
            perimeter += p;
        }
    }
    return {area, perimeter};
}

int64_t solve(std::vector<std::string> input) {
    int64_t result = 0;
    std::set<std::pair<int64_t, int64_t>> visited;
    for (size_t i = 0; i < input.size(); ++i) {
        for (size_t j = 0; j < input[0].size(); ++j) {
            if (!visited.contains({i, j})) {
                auto [area, perimeter] = fence_cost(input, i, j, input[i][j], visited);
                result += area * perimeter;
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

void pre_test() {
    const std::string input_text = R"_(OOOOO
OXOXO
OOOOO
OXOXO
OOOOO)_";
    std::istringstream is(input_text);
    const auto input = read_input(is);
    TEST(solve(input) == 772);
}

void test() {
    const std::string input_text = R"_(RRRRIICCFF
RRRRIICCCF
VVRRRCCFFF
VVRCCCJFFF
VVVVCJJCFE
VVIVCCJJEE
VVIIICJJEE
MIIIIIJJEE
MIIISIJEEE
MMMISSJEEE)_";
    std::istringstream is(input_text);
    const auto input = read_input(is);
    TEST(solve(input) == 1930);
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
    pre_test();
    test();
    run();
}

