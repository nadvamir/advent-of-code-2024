#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <ranges>
#include <set>
#include <map>
#include <unordered_map>
#include <utility>

int64_t calculate_area(auto& input, int64_t si, int64_t sj, char c, auto& visited, int64_t area_id, auto& walls) {
    int64_t area = 0;

    auto out_of_bounds = [&input](int64_t i, int64_t j) { return i < 0 || j < 0 || i >= input.size() || j >= input[i].size(); };
    constexpr std::array<std::pair<int, int>, 4> directions{{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}};

    std::queue<std::tuple<int64_t, int64_t>> q;
    q.push({si, sj});

    while (!q.empty()) {
        auto [i, j] = q.front();
        q.pop();

        if (visited.contains({i, j})) continue;
        visited[{i, j}] = area_id;

        ++area;

        for (size_t dir_idx = 0; dir_idx < 4; ++dir_idx) {
            auto [di, dj] = directions[dir_idx];
            auto ni = i + di, nj = j + dj;
            if (out_of_bounds(ni, nj) || input[ni][nj] != c) {
                walls[area_id].insert({ni, nj, dir_idx});
            }
            else if (!visited.contains({ni, nj})) {
                q.push({ni, nj});
            }
        }
    }
    return area;
}

int64_t count_walls(auto& walls) {
    constexpr std::array<std::pair<int, int>, 4> directions{{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}};
    int64_t sides = 0;

    while (!walls.empty()) {
        auto random_wall = begin(walls);
        auto [i, j, dir_idx] = *random_wall;
        walls.erase(random_wall);
        ++sides;

        for (auto [di, dj] : {directions[(dir_idx + 3) % 4], directions[(dir_idx + 1) % 4]}) {
            auto ni = i + di, nj = j + dj;
            while (walls.contains({ni, nj, dir_idx})) {
                walls.erase({ni, nj, dir_idx});
                ni += di;
                nj += dj;
            }
        }
    }

    return sides;
}

int64_t solve(std::vector<std::string> input) {
    std::map<std::pair<int64_t, int64_t>, int64_t> visited;
    std::map<int64_t, std::set<std::tuple<int64_t, int64_t, int64_t>>> walls;
    std::unordered_map<int64_t, int64_t> areas;

    int64_t area_id = 0;
    for (size_t i = 0; i < input.size(); ++i) {
        for (size_t j = 0; j < input[0].size(); ++j) {
            if (!visited.contains({i, j})) {
                areas[area_id] = calculate_area(input, i, j, input[i][j], visited, area_id, walls);
                ++area_id;
            }
        }
    }

    int64_t result = 0;
    for (const auto [id, area] : areas) {
        result += area * count_walls(walls[id]);
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
    const std::string input_text = R"_(AAAAAA
AAABBA
AAABBA
ABBAAA
ABBAAA
AAAAAA)_";
    std::istringstream is(input_text);
    const auto input = read_input(is);
    TEST(solve(input) == 368);
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
    TEST(solve(input) == 1206);
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

