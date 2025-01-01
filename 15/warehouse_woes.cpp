#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <ranges>

auto parse_input(const std::vector<std::string>& input) {
    auto empty_line = std::ranges::find(input, "");
    auto map = std::vector<std::string>(begin(input), empty_line);

    std::ostringstream oss;
    copy(next(empty_line), end(input), std::ostream_iterator<std::string>(oss));
    auto directions = oss.str();

    return std::make_pair(map, directions);
}

auto convert_to_idx_directions(const std::string& directions) {
    std::vector<size_t> idx_directions;
    idx_directions.reserve(directions.size());
    for (const auto& c : directions) {
        switch (c) {
        case '^': idx_directions.push_back(0); break;
        case 'v': idx_directions.push_back(1); break;
        case '<': idx_directions.push_back(2); break;
        case '>': idx_directions.push_back(3); break;
        }
    }
    return idx_directions;
}

auto find_robot(const auto& map) -> std::pair<int64_t, int64_t> {
    for (size_t i = 0; i < map.size(); ++i) {
        for (size_t j = 0; j < map[i].size(); ++j) {
            if (map[i][j] == '@') {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

void run_simulation(auto& map, const auto& directions) {
    auto out_of_bounds = [&map](int64_t i, int64_t j) { return i < 0 || j < 0 || i >= map.size() || j >= map[i].size(); };
    auto dirs = std::array<std::pair<int64_t, int64_t>, 4>{{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};

    auto shift_boxes = [&](int64_t i, int64_t j, int64_t di, int64_t dj) {
        auto ni = i, nj = j;
        while (!out_of_bounds(ni, nj) && map[ni][nj] == 'O') {
            ni += di;
            nj += dj;
        }
        if (out_of_bounds(ni, nj) || map[ni][nj] == '#') return false;
        while (i != ni || j != nj) {
            map[ni][nj] = 'O';
            ni -= di;
            nj -= dj;
        }
        map[i][j] = '.';
        return true;
    };

    auto [i, j] = find_robot(map);
    map[i][j] = '.';
    for (size_t dir : directions) {
        auto [di, dj] = dirs[dir];
        auto ni = i + di, nj = j + dj;
        if (out_of_bounds(ni, nj)) continue;
        switch (map[ni][nj]) {
        case '#': continue;
        case '.': i = ni; j = nj; break;
        case 'O': 
            if (shift_boxes(ni, nj, di, dj)) {
                i = ni; j = nj;
            }
            break;
        }
    }
}

auto calc_gps(const auto& map) {
    int64_t result = 0;
    for (size_t i = 0; i < map.size(); ++i) {
        for (size_t j = 0; j < map[i].size(); ++j) {
            if (map[i][j] == 'O') {
                result += i * 100 + j;
            }
        }
    }
    return result;
}

int64_t solve(const std::vector<std::string>& input) {
    auto [map, char_directions] = parse_input(input);
    auto directions = convert_to_idx_directions(char_directions);
    run_simulation(map, directions);
    return calc_gps(map);
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
    const std::string input_text = R"_(########
#..O.O.#
##@.O..#
#...O..#
#.#.O..#
#...O..#
#......#
########

<^^>>>vv<v>>v<<
)_";
    std::istringstream is(input_text);
    const auto input = read_input(is);
    TEST(solve(input) == 2028);
}

void test() {
    const std::string input_text = R"_(##########
#..O..O.O#
#......O.#
#.OO..O.O#
#..O@..O.#
#O#..O...#
#O..O..O.#
#.OO.O.OO#
#....O...#
##########

<vv>^<v^>v>^vv^v>v<>v^v<v<^vv<<<^><<><>>v<vvv<>^v^>^<<<><<v<<<v^vv^v>^
vvv<<^>^v^^><<>>><>^<<><^vv^^<>vvv<>><^^v>^>vv<>v<<<<v<^v>^<^^>>>^<v<v
><>vv>v^v^<>><>>>><^^>vv>v<^^^>>v^v^<^^>v^^>v^<^v>v<>>v^v^<v>v^^<^^vv<
<<v<^>>^^^^>>>v^<>vvv^><v<<<>^^^vv^<vvv>^>v<^^^^v<>^>vvvv><>>v^<<^^^^^
^><^><>>><>^^<<^^v>>><^<v>^<vv>>v>>>^v><>^v><<<<v>>v<v<v>vvv>^<><<>^><
^>><>^v<><^vvv<^^<><v<<<<<><^v<<<><<<^^<v<^^^><^>>^<v^><<<^>>^v<v^v<v^
>^>>^v>vv>^<<^v<>><<><<v<<v><>v<^vv<<<>^^v^>^^>>><<^v>>v^v><^^>>^<>vv^
<><^^>^^^<><vvvvv^v<v<<>^v<v>v<<^><<><<><<<^^<<<^<<>><<><^^^>^^<>^>v<>
^^>vv<^v^v<vv>^<><v<^v>^^^>>>^^vvv^>vvv<>>>^<^>>>>>^<<^v>^vvv<>^<><<v>
v^^>>><<^^<>>^v^<v^vv<>v^<<>^<^v^v><^<<<><<^<v><v<>vv>>v><v^<vv<>v^<<^
)_";
    std::istringstream is(input_text);
    const auto input = read_input(is);
    TEST(solve(input) == 10092);
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

