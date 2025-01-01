#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <ranges>

auto parse_input(const std::vector<std::string>& input) {
    int64_t A, B, C, reg;
    std::string word;

    auto read_register = [&](const auto& line) {
        std::istringstream is(line);
        is >> word >> word >> reg;
        return reg;
    };
    A = read_register(input[0]);
    B = read_register(input[1]);
    C = read_register(input[2]);

    std::istringstream is(input[4]);
    is >> word >> word;
    std::vector<int> program;
    for (size_t i = 0; i < word.size(); i += 2) {
        program.emplace_back(word[i] - '0');
    }

    return std::tuple{A, B, C, program};
}

std::string run(int64_t A, int64_t B, int64_t C, std::vector<int> program) {
    auto literal = [](int op) { return op; };
    auto combo = [&](int64_t op) { 
        switch (op) {
        case 0: case 1: case 2: case 3: return op;
        case 4: return A;
        case 5: return B;
        case 6: return C;
        }
        return -1ll;
    };

    std::string result;
    for (int64_t ip = 0; ip + 1 < program.size(); ip += 2) {
        const auto op = program[ip + 1];
        switch(program[ip]) {
        case 0: A = A / (1ll << combo(op)); break; // adv
        case 1: B = B ^ literal(op); break; // bxl
        case 2: B = combo(op) & 7; break; // bst
        case 3: if (A) ip = literal(op) - 2; break; // jnz
        case 4: B = B ^ C; break; // bxc
        case 5: result += std::to_string(combo(op) & 7) + ","; break; // out
        case 6: B = A / (1ll << combo(op)); break; // bdv
        case 7: C = A / (1ll << combo(op)); break; // cdv
        }
        //std::cout << program[ip] << " " << program[ip + 1] << ": " << A << " " << B << " " << C << "\n";
    }
    return result.substr(0, result.size() - 1);
}

std::string solve(const std::vector<std::string>& input) {
    auto [A, B, C, program] = parse_input(input);
    return run(A, B, C, program);
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
    TEST(run(10, 0, 0, {5,0,5,1,5,4}) == "0,1,2");
    TEST(run(729, 0, 0, {0,1,5,4,3,0}) == "4,6,3,5,6,3,5,2,1,0");
}

void test() {
    const std::string input_text = R"_(Register A: 729
Register B: 0
Register C: 0

Program: 0,1,5,4,3,0
)_";
    std::istringstream is(input_text);
    const auto input = read_input(is);
    TEST(solve(input) == "4,6,3,5,6,3,5,2,1,0");
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

