#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <ranges>
#include <set>

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
    std::vector<char> program;
    for (size_t i = 0; i < word.size(); i += 2) {
        program.emplace_back(word[i] - '0');
    }

    return std::tuple{A, B, C, program};
}

bool run_until(int64_t A, int64_t B, int64_t C, const auto& program, size_t expected, bool fast) {
    auto literal = [](int op) { return op; };
    auto combo = [&](char op) -> int64_t { 
        switch (op) {
        case 0: case 1: case 2: case 3: return op;
        case 4: return A;
        case 5: return B;
        case 6: return C;
        case 7: std::terminate();
        }
        return -1ll;
    };

    size_t num_outputs = 0;
    if (fast) {
        A >>= expected * 3;
        num_outputs = expected;
    }

    for (int64_t ip = 0; ip + 1 < program.size(); ip += 2) {
        const auto op = program[ip + 1];
        switch(program[ip]) {
        case 0: A = A >> combo(op); break; // adv
        case 1: B = B ^ literal(op); break; // bxl
        case 2: B = combo(op) & 7; break; // bst
        case 3: if (A) ip = literal(op) - 2; break; // jnz
        case 4: B = B ^ C; break; // bxc
        case 5:
            if ((combo(op) & 7) != program[num_outputs]) return false;
            if (num_outputs++ == expected) return true;
            break; // out
        case 6: B = A >> combo(op); break; // bdv
        case 7: C = A >> combo(op); break; // cdv
        }
    }
    return false;
}

size_t num_bits(int64_t n) {
    size_t count = 0;
    while (n) {
        ++count;
        n >>= 1;
    }
    return count;
}

void print_bits(int64_t n) {
    // prints bits of the number, most significant bit first
    size_t num_bits = 32;
    for (size_t i = 0; i < num_bits; ++i) {
        std::cout << ((n & (1ll << (num_bits - i - 1))) ? '1' : '0');
    }
}

int64_t search(
    int64_t A,
    int64_t B,
    int64_t C,
    const auto& program,
    size_t i,
    int64_t result,
    int64_t min_result,
    auto& visited)
{
    if (i == program.size()) {
        return std::min(min_result, result);
    }

    const auto rbits = num_bits(result);
    const auto max_to_check = 10 + 3 * i;
    const auto max_A = 1ll << (max_to_check - rbits);

    while (A < max_A) {
        if (visited.contains({A, result, i})) {
            return min_result;
        }
        visited.insert({A, result, i});

        while (A < max_A
                && (!run_until((A << rbits) | result, B, C, program, i, true)
                    || !run_until((A << rbits) | result, B, C, program, i, false))) {
            ++A;
        }

        if (A < max_A) {
            // Found a potential solution
            min_result = search(0, B, C, program, i + 1, (A << rbits) | result, min_result, visited);
            ++A;
        }
    }

    return min_result;
}

int64_t solve(const std::vector<std::string>& input) {
    /*
    Program: 2,4,1,1,7,5,0,3,4,7,1,6,5,5,3,0
    B = A & 7; // 2,4
    B = B ^ 1; // 1,1
    C = A >> B; // 7,5
    A = A >> 3; // 0,3
    B = B ^ C // 4,7
    B = B ^ 6 // 1,6
    out B & 7 // 5,5
    if A ip = 0 // 3,0

    Test program:    
    0,3,5,4,3,0
    A >> 3 // 0,3
    out A & 7 // 5,4
    while A // 3,0
    */

    auto [A, B, C, program] = parse_input(input);
    int64_t result = 0, min_result = std::numeric_limits<int64_t>::max();

    std::set<std::tuple<int64_t, int64_t, int64_t>> visited;

    min_result = search(0, B, C, program, 0, result, min_result, visited);

    std::cout << "A: " << min_result << "\n";
    std::cout << "Validation: " << run_until(min_result, B, C, program, program.size() - 1, false) << "\n";
    std::cout << "Validation 2: " << (min_result == 247839002892474) << "\n";
    return min_result;
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
    const std::string input_text = R"_(Register A: 2024
Register B: 0
Register C: 0

Program: 0,3,5,4,3,0
)_";
    std::istringstream is(input_text);
    const auto input = read_input(is);
    TEST(solve(input) == 117440);
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

