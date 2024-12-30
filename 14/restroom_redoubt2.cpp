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

double calc_entropy(const std::vector<int64_t>& image) {
    std::array<double, 256> hist = {0};
    for (auto count : image) {
        if (count >= 256) continue;
        ++hist[count];
    }

    double entropy = 0;
    for (auto p : hist) {
        p /= image.size();
        if (p > 0) entropy -= p * std::log2(p);
    }
    return entropy;
}

int64_t solve(const std::vector<std::string>& input, int64_t W, int64_t H) {
    auto robots = parse_input(input);
    double min_entropy = std::numeric_limits<double>::max();
    int64_t min_entropy_t = 0;
    std::vector<int64_t> tree_img(W * H);

    for (size_t t = 1; t <= W * H; ++t) {
        std::vector<int64_t> image(W * H);
        for (auto& [pos, velocity] : robots) {
            simulate(pos, velocity, 1, W, H);
            image[pos.y * W + pos.x] += 1;
        }
        
        const auto entropy = calc_entropy(image);
        if (entropy < min_entropy) {
            min_entropy = entropy;
            min_entropy_t = t;
            tree_img = image;
        }
    }

    for (size_t i = 0; i < H; ++i) {
        for (size_t j = 0; j < W; ++j) {
            std::cout << (tree_img[i * W + j] > 0 ? '#' : '.');
        }
        std::cout << "\n";
    }

    std::cout << "Entropy: " << min_entropy << "\n";
    return min_entropy_t;
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
    //TEST(solve(input, 11, 7) == 12);
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

