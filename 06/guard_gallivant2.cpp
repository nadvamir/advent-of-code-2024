#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <ranges>
#include <unordered_set>

int64_t solve(const std::vector<std::string>& input) {
    using namespace std::ranges;
    auto map = input;
    auto s_row = find_if(map, [](const auto& row) { return row.find('^') != std::string::npos; });
    int64_t i = s_row - begin(map), j = s_row->find('^');

    auto out_of_bounds = [&map](int64_t i, int64_t j) {
        return i < 0 || j < 0 || i >= map.size() || j >= map[i].size();
    };

    auto next = [](int64_t i, int64_t j, char rotation) {
        switch (rotation) {
        case '^': return std::make_tuple(i - 1, j, '>');
        case 'v': return std::make_tuple(i + 1, j, '<');
        case '<': return std::make_tuple(i, j - 1, '^');
        case '>': return std::make_tuple(i, j + 1, 'v');
        }
        return std::make_tuple(i, j, rotation);
    };

    auto hash = [&map](int64_t i, int64_t j, char rotation) {
        auto N = map.size(), M = map[0].size();
        switch (rotation) {
        case '^': return i * M + j;
        case 'v': return 1 * N * M + i * M + j;
        case '<': return 2 * N * M + i * M + j;
        case '>': return 3 * N * M + i * M + j;
        }
        return 4 * N * M + i * M + j;
    };

    auto leads_to_cycle = [&](const auto& map, int64_t i, int64_t j, char rotation, auto visited) {
        do {
            auto loc = hash(i, j, rotation);
            if (visited.contains(loc)) return true;
            visited.insert(loc);

            auto [next_i, next_j, next_rotation] = next(i, j, rotation);
            if (out_of_bounds(next_i, next_j)) return false;
            if (map[next_i][next_j] == '#') {
                rotation = next_rotation;
            }
            else {
                i = next_i, j = next_j;
            }
        }
        while (true);
    };

    std::unordered_set<int64_t> obstacles, visited;
    char rotation = map[i][j];
    map[i][j] = '.';
    while (true) {
        visited.insert(hash(i, j, rotation));
        auto [next_i, next_j, next_rotation] = next(i, j, rotation);
        if (out_of_bounds(next_i, next_j)) {
            return obstacles.size();
        }
        if (map[next_i][next_j] == '#') {
            rotation = next_rotation;
        }
        else {
            map[next_i][next_j] = '#';
            if (!obstacles.contains(hash(next_i, next_j, 'O')) && leads_to_cycle(map, i, j, next_rotation, visited)) {
                const auto locs = {hash(next_i, next_j, '^'), hash(next_i, next_j, 'v'), hash(next_i, next_j, '<'), hash(next_i, next_j, '>')};
                if (none_of(locs, [&visited](int64_t loc) { return visited.contains(loc); })) {
                    obstacles.insert(hash(next_i, next_j, 'O'));
                }
            }
            map[next_i][next_j] = '.';
            i = next_i, j = next_j;
        }
    }
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
    const std::string input_text = R"_(....#.....
.........#
..........
..#.......
.......#..
..........
.#..^.....
........#.
#.........
......#...)_";
    std::istringstream is(input_text);
    const auto input = read_input(is);
    TEST(solve(input) == 6);
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

