#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <ranges>
#include <queue>
#include <map>
#include <tuple>

int64_t solve(std::vector<std::string> input) {
    auto directions = std::array<std::pair<int64_t, int64_t>, 4>{{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}};
    auto out_of_bounds = [&](int64_t i, int64_t j) {
        return i < 0 || j < 0 || i >= input.size() || j >= input[i].size();
    };

    using El = std::tuple<int64_t, int64_t, int64_t, int64_t>;
    auto q = std::priority_queue<El, std::vector<El>, std::greater<El>>{};
    q.push({0, input.size() - 2, 1, 0});

    std::map<std::tuple<int64_t, int64_t, int64_t>, int64_t> visited;

    while (!q.empty()) {
        auto [dist, i, j, dir] = q.top();
        q.pop();
        if (input[i][j] == 'E') {
            return dist;
        }
        auto visited_it = visited.find({i, j, dir});
        if (visited_it != end(visited) && visited_it->second <= dist) {
            continue;
        }
        visited[{i, j, dir}] = dist;

        auto move = [&](auto next_dir, auto cost) {
            auto [di, dj] = directions[next_dir];
            if (out_of_bounds(i + di, j + dj)) return;
            if (input[i + di][j + dj] == '#') return;

            if (next_dir == dir) {
                q.push({dist + cost, i + di, j + dj, next_dir});
            }
            else {
                // just rotate
                q.push({dist + cost, i, j, next_dir});
            }
        };
        move(dir, 1);
        move((dir + 1) % 4, 1000);
        move((dir + 3) % 4, 1000);
    }
    std::cout << "PPath not found" << std::endl;
    return -1;
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
    const std::string input_text = R"_(#################
#...#...#...#..E#
#.#.#.#.#.#.#.#.#
#.#.#.#...#...#.#
#.#.#.#.###.#.#.#
#...#.#.#.....#.#
#.#.#.#.#.#####.#
#.#...#.#.#.....#
#.#.#####.#.###.#
#.#.#.......#...#
#.#.###.#####.###
#.#.#...#.....#.#
#.#.#.#####.###.#
#.#.#.........#.#
#.#.#.#########.#
#S#.............#
#################)_";
    std::istringstream is(input_text);
    const auto input = read_input(is);
    TEST(solve(input) == 11048);
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

