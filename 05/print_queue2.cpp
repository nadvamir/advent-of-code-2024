#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <ranges>
#include <unordered_map>
#include <unordered_set>

struct Node {
    int64_t degree{0};
    std::unordered_set<int64_t> children;
};

auto parse_input(const std::vector<std::string>& input) {
    using namespace std::views;
    using namespace std::ranges;
    using std::views::transform;

    const auto split_point = find_if(input, [](const auto& line) { return line.empty(); });
    const auto ordering_lines = subrange(begin(input), split_point);
    const auto update_lines = subrange(next(split_point), end(input));

    std::unordered_map<int64_t, Node> ordering;
    auto parse_ordering = [&ordering](const std::string& line) {
        std::istringstream is(line);
        int64_t before, after;
        char c;
        is >> before >> c >> after;
        ordering[after].degree++;
        ordering[before].children.insert(after);
    };
    for_each(ordering_lines, parse_ordering);

    auto parse_update = [](const std::string& line) {
        std::istringstream is(line);
        std::string token;
        std::vector<int64_t> result;
        while (getline(is, token, ',')) {
            result.push_back(std::stoi(token));
        }
        return result;
    };
    auto updates = update_lines | transform(parse_update) | to<std::vector>();

    return std::make_pair(ordering, updates);
}

auto correct_the_update(auto& update, const auto& ordering) {
    auto bi = find_if(update.begin(), update.end(), [&ordering](int64_t val) { return ordering.contains(val); });
    if (bi == update.end()) return true;

    for (auto ai = next(bi); ai != update.end(); ++ai) {
        if (!ordering.contains(*ai)) continue;

        auto after = ordering.find(*ai);
        if (after->second.children.contains(*bi)) {
            stable_sort(begin(update), end(update), [&](int64_t a, int64_t b) {
                auto ao = ordering.find(a);
                auto bo = ordering.find(b);
                return ao == end(ordering) || bo == end(ordering) || !ao->second.children.contains(b);
            });
            return true;
        }
        bi = ai;
    }
    return false;
}

int64_t solve(const std::vector<std::string>& input) {
    auto [ordering, updates] = parse_input(input);

    int64_t result = 0;
    size_t i = 0;
    for (auto& update : updates) {
        if (correct_the_update(update, ordering)) {
            result += update[update.size() / 2];
        }
        ++i;
    }
    return result;
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
    const std::string input_text = R"_(47|53
97|13
97|61
97|47
75|29
61|13
75|53
29|13
97|29
53|29
61|53
97|53
61|29
47|13
75|47
97|75
47|61
75|61
47|29
75|13
53|13

75,47,61,53,29
97,61,53,29,13
75,29,13
75,97,47,61,53
61,13,29
97,13,75,29,47)_";
    std::istringstream is(input_text);
    const auto input = read_input(is);
    TEST(solve(input) == 123);
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

