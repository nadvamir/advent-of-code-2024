#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <ranges>
#include <set>
#include <unordered_map>

using Point = std::pair<int64_t, int64_t>;
using Points = std::set<Point>;

auto discover_antennas(const std::vector<std::string>& input) {
    std::unordered_map<char, Points> antennas;
    for (size_t i = 0; i < input.size(); ++i) {
        for (size_t j = 0; j < input[i].size(); ++j) {
            if (input[i][j] != '.') {
                antennas[input[i][j]].insert({i, j});
            }
        }
    }
    return antennas;
}

int64_t solve(const std::vector<std::string>& input) {
    const auto antennas = discover_antennas(input);
    const auto N = input.size(), M = input[0].size();
    Points antinodes;

    for (const auto& [_, points] : antennas) {
        for (size_t a = 0; a < points.size(); ++a) {
            for (size_t b = a + 1; b < points.size(); ++b) {
                const auto& [y1, x1] = *next(points.begin(), a);
                const auto& [y2, x2] = *next(points.begin(), b);
                int64_t dx = x2 - x1, dy = y2 - y1;
                if (x1 - dx >= 0 && x1 - dx < M && y1 - dy >= 0 && y1 - dy < N) {
                    antinodes.insert({y1 - dy, x1 - dx});
                }
                if (x2 + dx >= 0 && x2 + dx < M && y2 + dy >= 0 && y2 + dy < N) {
                    antinodes.insert({y2 + dy, x2 + dx});
                }
            }
        }
    }

    return antinodes.size();
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
    const std::string input_text = R"_(............
........0...
.....0......
.......0....
....0.......
......A.....
............
............
........A...
.........A..
............
............)_";
    std::istringstream is(input_text);
    const auto input = read_input(is);
    TEST(solve(input) == 14);
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

