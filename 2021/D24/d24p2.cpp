#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

struct Block {
    int div_z = 0;
    int add_x = 0;
    int add_y = 0;
};

static bool starts_with(const std::string& s, const std::string& p) {
    return s.size() >= p.size() && s.compare(0, p.size(), p) == 0;
}

static int last_int(const std::string& s) {
    int v = 0;
    std::sscanf(s.c_str(), "%*s %*s %d", &v);
    return v;
}

int main() {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        lines.push_back(line);
    }
    if (lines.empty()) return 0;

    std::vector<std::vector<std::string>> blocks;
    for (const auto& ln : lines) {
        if (starts_with(ln, "inp ")) {
            blocks.emplace_back();
        }
        if (blocks.empty()) return 0;
        blocks.back().push_back(ln);
    }
    if (blocks.size() != 14) return 0;

    std::vector<Block> params;
    params.reserve(14);
    for (const auto& b : blocks) {
        if (b.size() != 18) return 0;
        Block blk;
        blk.div_z = last_int(b[4]);   // div z A
        blk.add_x = last_int(b[5]);   // add x B
        blk.add_y = last_int(b[15]);  // add y C
        params.push_back(blk);
    }

    std::vector<int> digits(14, 0);
    std::vector<std::pair<int, int>> st;
    std::vector<std::tuple<int, int, int>> cons; // (j, i, delta)

    for (int i = 0; i < 14; i++) {
        const auto& b = params[i];
        if (b.div_z == 1) {
            st.push_back({i, b.add_y});
        } else {
            if (st.empty()) return 0;
            const auto [j, yj] = st.back();
            st.pop_back();
            const int delta = yj + b.add_x;
            cons.push_back({j, i, delta});
        }
    }
    if (!st.empty()) return 0;

    // Minimize the 14-digit number lexicographically (leftmost digit prioritized).
    for (const auto& [j, i, delta] : cons) {
        const int lo = std::max(1, 1 - delta);
        const int hi = std::min(9, 9 - delta);
        const int dj = lo;
        const int di = dj + delta;
        if (dj < lo || dj > hi || di < 1 || di > 9) return 0;
        digits[j] = dj;
        digits[i] = di;
    }

    for (int d : digits) std::cout << d;
    std::cout << std::endl;
    return 0;
}

