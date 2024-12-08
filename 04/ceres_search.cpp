#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <ranges>

struct WordCounter {
    std::string word;
    size_t idx{0};

    bool feed(char c) {
        if (word[idx] == c) {
            ++idx;
            idx %= word.size();
            return idx == 0;
        } else {
            idx = word[0] == c;
            return false;
        }
    }
};

int64_t solve(const std::vector<std::string>& input) {
    if (input.empty()) return 0;

    size_t N = input.size(), M = input[0].size();
    std::vector<WordCounter> horiz_words(N, {"XMAS"}), rhoriz_words(N, {"SAMX"});
    std::vector<WordCounter> vert_words(M, {"XMAS"}), rvert_words(M, {"SAMX"});
    std::vector<WordCounter> ldiag_words(N + M - 1, {"XMAS"}), rldiag_words(N + M - 1, {"SAMX"});
    std::vector<WordCounter> rdiag_words(N + M - 1, {"XMAS"}), rrdiag_words(N + M - 1, {"SAMX"});

    int64_t count = 0;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            char c = input[i][j];
            count += horiz_words[i].feed(c);
            count += rhoriz_words[i].feed(c);
            count += vert_words[j].feed(c);
            count += rvert_words[j].feed(c);

            size_t md = std::min(i, j);
            size_t di = i - md, dj = j - md;
            if (di == 0) dj += N;
            count += ldiag_words[di + dj].feed(c);
            count += rldiag_words[di + dj].feed(c);

            size_t ri = i, rj = M - j - 1;
            md = std::min(ri, rj);
            di = ri - md, dj = rj - md;
            if (di == 0) dj += N;
            count += rdiag_words[di + dj].feed(c);
            count += rrdiag_words[di + dj].feed(c);
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
    TEST(solve(input) == 18);
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

