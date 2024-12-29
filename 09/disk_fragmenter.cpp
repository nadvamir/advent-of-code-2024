#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
#include <ranges>
#include <utility>

using ID = size_t;
using Size = size_t;
using File = std::pair<ID, Size>;

std::vector<File> defragment(std::string disk_map) {
    std::vector<File> defragmented;
    defragmented.reserve(disk_map.size());
    for (size_t l = 0, r = disk_map.size() - 1; l <= r;) {
        while (disk_map[l] == '0' && l <= r) ++l;
        while ((disk_map[r] == '0' || r % 2) && l <= r) --r;
        size_t ln = disk_map[l] - '0', rn = disk_map[r] - '0';
        if (l % 2 == 0) {
            defragmented.emplace_back(l / 2, ln);
            ++l;
        }
        else {
            if (ln > rn) {
                defragmented.emplace_back(r / 2, rn);
                --r;
                disk_map[l] = '0' + ln - rn;
            }
            else if (rn > ln) {
                defragmented.emplace_back(r / 2, ln);
                ++l;
                disk_map[r] = '0' + rn - ln;
            }
            else {
                defragmented.emplace_back(r / 2, ln);
                ++l;
                --r;
            }
        }
    }
    return defragmented;
}

size_t checksum(const auto& defragmented) {
    size_t result = 0, idx = 0;;
    for (const auto [file_id, size] : defragmented) {
        result += file_id * size * (idx + idx + size - 1) / 2;
        idx += size;
    }
    return result;
}

int64_t solve(const std::vector<std::string>& input) {
    const auto defragmented = defragment(input[0]);
    return checksum(defragmented);
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
    const std::string input_text = R"_(2333133121414131402)_";
    std::istringstream is(input_text);
    const auto input = read_input(is);
    TEST(solve(input) == 1928);
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

