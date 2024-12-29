#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <cassert>
#include <algorithm>
#include <ranges>
#include <utility>

using ID = size_t;
using Size = size_t;
using File = std::pair<ID, Size>;

auto parse_input(std::string disk_map) {
    std::list<File> defragmented;
    for (size_t i = 0; i < disk_map.size(); ++i) {
        if (disk_map[i] == '0') continue;
        const auto size = disk_map[i] - '0';
        const auto file_id = i % 2 ? 0 : i / 2;
        defragmented.emplace_back(file_id, size);
    }
    return defragmented;
}

auto defragment(std::string disk_map) {
    auto defragmented = parse_input(std::move(disk_map));

    auto rightmost = prev(defragmented.end());
    if (!rightmost->first) --rightmost;
    for (auto l = next(begin(defragmented)), r = rightmost; l != r; --r) {
        auto [rid, rn] = *r;
        
        // Don't move empty spaces
        if (rid == 0) continue;
    
        // Don't consider files that are already in the right place
        // Purely for optimisation reasons
        while (l->first > 0 && l != r) ++l;
        if (l == r) break;

        // find a slot to place the file
        auto slot_l = l;
        while (slot_l != r && (slot_l->first > 0 || slot_l->second < rn)) ++slot_l;
        // keep the R in place if no slot found
        if (slot_l == r) continue;

        // can place now
        auto [lid, ln] = *slot_l;
        r->first = 0;
        if (ln > rn) {
            defragmented.insert(slot_l, {rid, rn});
            slot_l->second = ln - rn;
        }
        else {
            slot_l->first = rid;
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
    TEST(solve(input) == 2858);
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

