#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <ranges>
#include <vector>
#include <queue>
#include <unordered_set>
#include <tuple>

struct Coord {
    int64_t i, j;
};

auto parse_input(const std::vector<std::string>& input) {
    using namespace std::ranges;
    auto parse_line = [](const std::string& line) {
        std::istringstream is(line);
        Coord coord;
        char c;
        is >> coord.j >> c >> coord.i;
        return coord;
    };

    return input | std::views::transform(parse_line) | to<std::vector<Coord>>();
}

int64_t shortest_path(const auto& map) {
    const auto N = map.size(), M = map[0].size();

    auto q = std::queue<std::tuple<int64_t, int64_t, int64_t>>();
    auto visited = std::unordered_set<int64_t>();
    q.push({0, 0, 0});

    while (!q.empty()) {
        auto [i, j, dist] = q.front();
        q.pop();
        
        if (i < 0 || j < 0 || i >= N || j >= M) continue;
        if (map[i][j] == '#') continue;
        if (visited.contains(i * M + j)) continue;

        if (i == N - 1 && j == M - 1) return dist;

        visited.insert(i * M + j);

        q.push({i + 1, j, dist + 1});
        q.push({i - 1, j, dist + 1});
        q.push({i, j + 1, dist + 1});
        q.push({i, j - 1, dist + 1});
    }

    return -1;
}

std::string solve(const std::vector<std::string>& input, int64_t N, int64_t M, int64_t known_good) {
    auto bytes = parse_input(input);
    
    std::vector<std::string> map(N, std::string(M, '.'));
    for (const auto& byte : bytes) {
        map[byte.i][byte.j] = '#';
        if (--known_good < 0 && shortest_path(map) == -1) {
            std::cout << "Validation: 46,23 == " << byte.j << "," << byte.i << std::endl;
            return std::to_string(byte.j) + "," + std::to_string(byte.i);
        }
    }

    return "NOT FOUND";
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
    const std::string input_text = R"_(5,4
4,2
4,5
3,0
2,1
6,3
2,4
1,5
0,6
3,3
2,6
5,1
1,2
5,5
2,5
6,5
1,4
0,4
6,4
1,1
6,1
1,0
0,5
1,6
2,0)_";
    std::istringstream is(input_text);
    const auto input = read_input(is);
    TEST(solve(input, 7, 7, 12) == "6,1");
}

void run() {
    const auto input = read_input(std::cin);

    using namespace std::chrono;
    const auto start_ts = high_resolution_clock::now();

    std::cout << solve(input, 71, 71, 1024) << "\n";
    
    const auto end_ts = high_resolution_clock::now();
    std::cout << "Elapsed time: " << duration_cast<microseconds>(end_ts - start_ts).count() << "us\n";
}

int main() {
    test();
    run();
}

