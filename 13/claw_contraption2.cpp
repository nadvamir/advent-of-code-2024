#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <ranges>

struct Coord {
    int64_t x, y;
};

auto parse_input(const std::vector<std::string>& input) {
    std::vector<std::tuple<Coord, Coord, Coord>> games;

    Coord a, b, p;
    std::string word;
    char c;

    for (size_t i = 0; i < input.size(); i += 4) {
        std::istringstream a_is(input[i]);
        std::istringstream b_is(input[i + 1]);
        std::istringstream p_is(input[i + 2]);

        a_is >> word >> word >> c >> c >> a.x >> c >> c >> c >> a.y;
        b_is >> word >> word >> c >> c >> b.x >> c >> c >> c >> b.y;
        p_is >> word >> c >> c >> p.x >> c >> c >> c >> p.y;
        p.x += 10000000000000;
        p.y += 10000000000000;

        games.emplace_back(a, b, p);
    }

    return games;
}

int64_t cost(const auto& a, const auto& b, const auto& p) {
    const int64_t M_nom = (p.y * a.x - p.x * a.y);
    const int64_t M_div = (a.x * b.y - a.y * b.x);

    if (M_div == 0) {
        // infinite solutions
        const auto nmax = p.x / a.x;
        const auto mmax = p.x / b.x;
        return (3 * nmax < mmax) ? 3 * nmax : mmax;
    }
    else if (M_nom % M_div == 0) {
        const auto M = M_nom / M_div;
        const auto N_nom = (p.x - M * b.x);
        const auto N_div = a.x;
        if (N_nom % N_div == 0) {
            const auto N = N_nom / N_div;
            return 3 * N + M;
        }
    }
    return 0;
}

int64_t solve(const std::vector<std::string>& input) {
    auto games = parse_input(input);
    return accumulate(begin(games), end(games), 0ll, [](const auto& acc, const auto& game) {
        const auto& [a, b, p] = game;
        return acc + cost(a, b, p);
    });
    return 0;
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
    const std::string input_text = R"_(Button A: X+94, Y+34
Button B: X+22, Y+67
Prize: X=8400, Y=5400

Button A: X+26, Y+66
Button B: X+67, Y+21
Prize: X=12748, Y=12176

Button A: X+17, Y+86
Button B: X+84, Y+37
Prize: X=7870, Y=6450

Button A: X+69, Y+23
Button B: X+27, Y+71
Prize: X=18641, Y=10279
)_";
    std::istringstream is(input_text);
    const auto input = read_input(is);
    //TEST(solve(input) == 480);
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

