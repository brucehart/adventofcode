#include <iostream>
#include <string>
#include <unordered_set>
#include <cstdint>

static std::int64_t pack(int x, int y) {
    return (static_cast<std::int64_t>(x) << 32) | static_cast<std::uint32_t>(y);
}

int main() {
    std::unordered_set<std::int64_t> dots;

    std::string line;
    bool reading_dots = true;
    char fold_axis = 0;
    int fold_value = 0;

    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            reading_dots = false;
            continue;
        }

        if (reading_dots) {
            const auto comma = line.find(',');
            const int x = std::stoi(line.substr(0, comma));
            const int y = std::stoi(line.substr(comma + 1));
            dots.insert(pack(x, y));
            continue;
        }

        if (fold_axis != 0) continue;
        const auto eq = line.find('=');
        fold_axis = line[eq - 1];
        fold_value = std::stoi(line.substr(eq + 1));
    }

    if (fold_axis == 0) {
        std::cout << dots.size() << std::endl;
        return 0;
    }

    std::unordered_set<std::int64_t> folded;
    folded.reserve(dots.size());

    for (std::int64_t p : dots) {
        int x = static_cast<int>(p >> 32);
        int y = static_cast<int>(p & 0xffffffff);
        if (fold_axis == 'x' && x > fold_value) x = 2 * fold_value - x;
        if (fold_axis == 'y' && y > fold_value) y = 2 * fold_value - y;
        folded.insert(pack(x, y));
    }

    std::cout << folded.size() << std::endl;
    return 0;
}
