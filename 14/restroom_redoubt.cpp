#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <ranges>
#include <numeric>

struct Coord {
    int64_t x, y;
};

auto parse_input(const std::vector<std::string>& input) {
    using namespace std::ranges;

    auto parse_line = [](const std::string& line) {
        std::istringstream is(line);
        char c; Coord p, v;
        is >> c >> c >> p.x >> c >> p.y >> c >> c >> v.x >> c >> v.y;
        return std::pair{p, v};
    };

    return input | std::views::transform(parse_line) | to<std::vector<std::pair<Coord, Coord>>>();
}

void simulate(auto& pos, auto velocity, int64_t steps, int64_t W, int64_t H) {
    pos.x = (pos.x + (velocity.x + W) * steps) % W;
    pos.y = (pos.y + (velocity.y + H) * steps) % H;
}

size_t get_quarter(const auto& pos, int64_t W, int64_t H) {
    if (pos.x == W / 2 || pos.y == H / 2) return 4;
    if (pos.x < W / 2) {
        return pos.y < H / 2 ? 0 : 2;
    }
    else {
        return pos.y < H / 2 ? 1 : 3;
    }
}

int64_t solve(const std::vector<std::string>& input, int64_t W, int64_t H) {
    auto robots = parse_input(input);
    int64_t quarters[5] = {0, 0, 0, 0, 0};
    for (auto& [pos, velocity] : robots) {
        simulate(pos, velocity, 100, W, H);
        ++quarters[get_quarter(pos, W, H)];
    }
    quarters[4] = 1;
    return accumulate(std::begin(quarters), std::end(quarters), 1ll, std::multiplies{});
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
    const std::string input_text = R"_(p=0,4 v=3,-3
p=6,3 v=-1,-3
p=10,3 v=-1,2
p=2,0 v=2,-1
p=0,0 v=1,3
p=3,0 v=-2,-2
p=7,6 v=-1,-3
p=3,0 v=-1,-2
p=9,3 v=2,3
p=7,3 v=-1,2
p=2,4 v=2,-3
p=9,5 v=-3,-3)_";
    std::istringstream is(input_text);
    const auto input = read_input(is);
    TEST(solve(input, 11, 7) == 12);
}

void run() {
    const auto input = read_input(std::cin);

    using namespace std::chrono;
    const auto start_ts = high_resolution_clock::now();

    std::cout << solve(input, 101, 103) << "\n";
    
    const auto end_ts = high_resolution_clock::now();
    std::cout << "Elapsed time: " << duration_cast<microseconds>(end_ts - start_ts).count() << "us\n";
}

int main() {
    test();
    run();
}

