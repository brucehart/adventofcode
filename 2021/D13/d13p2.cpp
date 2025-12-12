#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cstdint>

static std::int64_t pack(int x, int y) {
    return (static_cast<std::int64_t>(x) << 32) | static_cast<std::uint32_t>(y);
}

static std::string glyph_key(const std::vector<std::string>& grid, int x0, int w) {
    std::string key;
    key.reserve(grid.size() * (w + 1));
    for (std::size_t y = 0; y < grid.size(); y++) {
        key.append(grid[y], x0, w);
        key.push_back('\n');
    }
    return key;
}

static std::unordered_map<std::string, char> build_font_4x6() {
    std::unordered_map<std::string, char> m;

    auto add = [&](char ch, const std::vector<std::string>& rows) {
        std::string key;
        for (const auto& r : rows) key += r + "\n";
        m[key] = ch;
    };

    add('A', {".##.", "#..#", "#..#", "####", "#..#", "#..#"});
    add('B', {"###.", "#..#", "###.", "#..#", "#..#", "###."});
    add('C', {".##.", "#..#", "#...", "#...", "#..#", ".##."});
    add('D', {"###.", "#..#", "#..#", "#..#", "#..#", "###."});
    add('E', {"####", "#...", "###.", "#...", "#...", "####"});
    add('F', {"####", "#...", "###.", "#...", "#...", "#..."});
    add('G', {".##.", "#..#", "#...", "#.##", "#..#", ".###"});
    add('H', {"#..#", "#..#", "####", "#..#", "#..#", "#..#"});
    add('I', {".##.", "..#.", "..#.", "..#.", "..#.", ".##."});
    add('J', {"..##", "...#", "...#", "...#", "#..#", ".##."});
    add('K', {"#..#", "#.#.", "##..", "#.#.", "#.#.", "#..#"});
    add('L', {"#...", "#...", "#...", "#...", "#...", "####"});
    add('M', {"#..#", "##.#", "#.#.", "#..#", "#..#", "#..#"});
    add('N', {"#..#", "##.#", "#.#.", "#..#", "#..#", "#..#"});
    add('O', {".##.", "#..#", "#..#", "#..#", "#..#", ".##."});
    add('P', {"###.", "#..#", "#..#", "###.", "#...", "#..."});
    add('Q', {".##.", "#..#", "#..#", "#..#", "#.##", ".###"});
    add('R', {"###.", "#..#", "#..#", "###.", "#.#.", "#..#"});
    add('S', {".###", "#...", "#...", ".##.", "...#", "###."});
    add('T', {"####", "..#.", "..#.", "..#.", "..#.", "..#."});
    add('U', {"#..#", "#..#", "#..#", "#..#", "#..#", ".##."});
    add('V', {"#..#", "#..#", "#..#", "#..#", ".#.#", "..#."});
    add('W', {"#..#", "#..#", "#..#", "#.#.", "##.#", "#..#"});
    add('X', {"#..#", ".#.#", "..#.", "..#.", ".#.#", "#..#"});
    add('Y', {"#..#", ".#.#", "..#.", "..#.", "..#.", "..#."});
    add('Z', {"####", "...#", "..#.", ".#..", "#...", "####"});

    return m;
}

int main() {
    std::unordered_set<std::int64_t> dots;
    std::vector<std::pair<char, int>> folds;

    std::string line;
    bool reading_dots = true;
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

        const auto eq = line.find('=');
        const char axis = line[eq - 1];
        const int value = std::stoi(line.substr(eq + 1));
        folds.push_back({axis, value});
    }

    for (const auto& f : folds) {
        std::unordered_set<std::int64_t> next;
        next.reserve(dots.size());
        for (std::int64_t p : dots) {
            int x = static_cast<int>(p >> 32);
            int y = static_cast<int>(p & 0xffffffff);
            if (f.first == 'x' && x > f.second) x = 2 * f.second - x;
            if (f.first == 'y' && y > f.second) y = 2 * f.second - y;
            next.insert(pack(x, y));
        }
        dots.swap(next);
    }

    if (dots.empty()) return 0;

    int min_x = INT32_MAX, min_y = INT32_MAX;
    int max_x = INT32_MIN, max_y = INT32_MIN;
    for (std::int64_t p : dots) {
        const int x = static_cast<int>(p >> 32);
        const int y = static_cast<int>(p & 0xffffffff);
        if (x < min_x) min_x = x;
        if (y < min_y) min_y = y;
        if (x > max_x) max_x = x;
        if (y > max_y) max_y = y;
    }

    const int width = max_x - min_x + 1;
    const int height = max_y - min_y + 1;

    std::vector<std::string> grid(height, std::string(width, '.'));
    for (std::int64_t p : dots) {
        const int x = static_cast<int>(p >> 32) - min_x;
        const int y = static_cast<int>(p & 0xffffffff) - min_y;
        grid[y][x] = '#';
    }

    std::string code;
    if (height == 6) {
        const auto font4 = build_font_4x6();
        for (int x = 0; x < width;) {
            bool blank_col = true;
            for (int y = 0; y < height; y++) {
                if (grid[y][x] != '.') {
                    blank_col = false;
                    break;
                }
            }
            if (blank_col) {
                x++;
                continue;
            }

            int x2 = x;
            while (x2 < width) {
                bool blank = true;
                for (int y = 0; y < height; y++) {
                    if (grid[y][x2] != '.') {
                        blank = false;
                        break;
                    }
                }
                if (blank) break;
                x2++;
            }
            const int glyph_w = x2 - x;

            char ch = '?';
            if (glyph_w == 4) {
                const std::string key = glyph_key(grid, x, 4);
                auto it = font4.find(key);
                if (it != font4.end()) ch = it->second;
            }
            code.push_back(ch);
            x = x2;
        }
    }

    bool ok = !code.empty();
    for (char c : code) ok = ok && (c >= 'A' && c <= 'Z');
    if (ok) {
        std::cout << code << std::endl;
        return 0;
    }

    for (const auto& row : grid) std::cout << row << "\n";
    return 0;
}

