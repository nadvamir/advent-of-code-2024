#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <ranges>

int64_t solve(const std::vector<std::string>& input) {
    auto map = input;
    auto s_row = std::ranges::find_if(map, [](const auto& row) { return row.find('^') != std::string::npos; });
    int64_t i = s_row - begin(map), j = s_row->find('^');
    
    int64_t count = 1;
    auto out_of_bounds = [&map](int64_t i, int64_t j) { return i < 0 || j < 0 || i >= map.size() || j >= map[i].size(); };
    while (true) {
        auto next_i = i, next_j = j;
        char rotation = map[i][j];
        switch (map[i][j]) {
        case '^': --next_i; rotation = '>'; break;
        case 'v': ++next_i; rotation = '<'; break;
        case '<': --next_j; rotation = '^'; break;
        case '>': ++next_j; rotation = 'v'; break;
        }
        if (out_of_bounds(next_i, next_j)) {
            return count;
        }
        if (map[next_i][next_j] == '#') {
            map[i][j] = rotation;
        }
        else {
            if (map[next_i][next_j] == '.') ++count;
            map[next_i][next_j] = map[i][j];
            map[i][j] = 'X';
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
    TEST(solve(input) == 41);
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

