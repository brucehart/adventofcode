#include <iostream>
#include <string>
#include <unordered_set>
#include <cstdint>
#include <algorithm>

static std::int64_t pack(int x, int y) {
    return (static_cast<std::int64_t>(x) << 32) | static_cast<std::uint32_t>(y);
}

static bool has(const std::unordered_set<std::int64_t>& s, int x, int y) {
    return s.find(pack(x, y)) != s.end();
}

int main() {
    std::string algo, line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) break;
        algo += line;
    }
    if (algo.size() != 512) return 0;

    std::unordered_set<std::int64_t> diff;
    bool bg = false; // false='.', true='#'

    int y = 0;
    int min_x = 0, min_y = 0, max_x = -1, max_y = -1;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        if (max_x < 0) {
            min_x = 0;
            min_y = 0;
        }
        max_x = std::max(max_x, static_cast<int>(line.size()) - 1);
        max_y = std::max(max_y, y);
        for (int x = 0; x < static_cast<int>(line.size()); x++) {
            if (line[x] == '#') diff.insert(pack(x, y));
        }
        y++;
    }
    if (max_x < 0) return 0;

    auto is_lit = [&](int x, int y) {
        const bool in = has(diff, x, y);
        return bg ? !in : in;
    };

    for (int step = 0; step < 2; step++) {
        const bool new_bg = bg ? (algo[511] == '#') : (algo[0] == '#');
        std::unordered_set<std::int64_t> next;

        const int nx0 = min_x - 1;
        const int nx1 = max_x + 1;
        const int ny0 = min_y - 1;
        const int ny1 = max_y + 1;

        for (int yy = ny0; yy <= ny1; yy++) {
            for (int xx = nx0; xx <= nx1; xx++) {
                int idx = 0;
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        idx = (idx << 1) | (is_lit(xx + dx, yy + dy) ? 1 : 0);
                    }
                }
                const bool out_lit = (algo[idx] == '#');
                if (!new_bg) {
                    if (out_lit) next.insert(pack(xx, yy));
                } else {
                    if (!out_lit) next.insert(pack(xx, yy));
                }
            }
        }

        diff.swap(next);
        bg = new_bg;
        min_x = nx0;
        max_x = nx1;
        min_y = ny0;
        max_y = ny1;
    }

    if (bg) return 0; // would be infinite lit
    std::cout << diff.size() << std::endl;
    return 0;
}

