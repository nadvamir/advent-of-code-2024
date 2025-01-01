#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <ranges>
#include <queue>
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <tuple>

struct State {
    int dist;
    int i, j, dir;
    std::unordered_set<int> path;

    bool operator>(const State& other) const {
        return std::tie(dist, i, j, dir) > std::tie(other.dist, other.i, other.j, other.dir);
    }
};

int solve(std::vector<std::string> input) {
    auto directions = std::array<std::pair<int, int>, 4>{{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}};
    auto out_of_bounds = [&](int i, int j) {
        return i < 0 || j < 0 || i >= input.size() || j >= input[i].size();
    };

    auto hash = [&input](int i, int j, int dir) {
        return i * input[0].size() * 10 + j * 10 + dir;
    };

    using El = State;
    auto q = std::priority_queue<El, std::vector<El>, std::greater<El>>{};
    q.push({0, (int) input.size() - 2, 1, 0, {}});

    std::unordered_map<int, int> visited;
    
    std::unordered_set<int> best_path;
    int best_dist = std::numeric_limits<int>::max();

    int ei = 1, ej = input[0].size() - 2;

    while (!q.empty()) {
        auto [dist, i, j, dir, path] = q.top();
        q.pop();

        // if there is no chance to beat the record, skip
        if (dist + std::abs(ei - i) + std::abs(ej - j) > best_dist) {
            continue;
        }

        if (auto it = visited.find(hash(i, j, dir)); it != end(visited) && it->second < dist) {
            continue;
        }
        visited[hash(i, j, dir)] = dist;

        path.insert(i * input[0].size() + j);

        if (input[i][j] == 'E') {
            if (dist < best_dist) {
                best_dist = dist;
                best_path = path;
            }
            else if (dist == best_dist) {
                best_path.insert(begin(path), end(path));
            }
            else {
                continue;
            }
        }

        auto move = [&](auto next_dir, auto ni, auto nj, auto cost) {
            if (out_of_bounds(ni, nj)) return;
            if (input[ni][nj] == '#') return;
            if (dist + std::abs(ei - ni) + std::abs(ej - nj) > best_dist) {
                return;
            }
            if (auto it = visited.find(hash(ni, nj, next_dir)); it != end(visited) && it->second < dist + cost) {
                return;
            }
            q.push({dist + cost, ni, nj, next_dir, path});
        };
        auto [di, dj] = directions[dir];
        move(dir, i + di, j + dj, 1);
        move((dir + 1) % 4, i, j, 1000);
        move((dir + 3) % 4, i, j, 1000);
    }

    return best_path.size();
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
    TEST(solve(input) == 64);
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

