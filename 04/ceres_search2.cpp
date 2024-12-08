#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <ranges>

int64_t solve(const std::vector<std::string>& input) {
    if (input.empty()) return 0;

    size_t N = input.size(), M = input[0].size();
    int64_t count = 0;

    for (size_t i = 1; i + 1 < N; ++i) {
        for (size_t j = 1; j + 1 < M; ++j) {
            if (input[i][j] != 'A') continue;
            const bool d1 = input[i-1][j-1] == 'M' && input[i+1][j+1] == 'S' || input[i-1][j-1] == 'S' && input[i+1][j+1] == 'M';
            const bool d2 = input[i-1][j+1] == 'M' && input[i+1][j-1] == 'S' || input[i-1][j+1] == 'S' && input[i+1][j-1] == 'M';
            count += d1 && d2;
        }
    }
    return count;
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
    const std::string input_text = R"_(MMMSXXMASM
MSAMXMSMSA
AMXSXMAAMM
MSAMASMSMX
XMASAMXAMM
XXAMMXXAMA
SMSMSASXSS
SAXAMASAAA
MAMMMXMMMM
MXMXAXMASX)_";
    std::istringstream is(input_text);
    const auto input = read_input(is);
    TEST(solve(input) == 9);
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

