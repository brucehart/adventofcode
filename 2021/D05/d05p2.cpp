#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Line {
    int x1, y1, x2, y2;
};

int main() {
    std::string lineStr;
    std::vector<Line> lines;
    int maxX = 0, maxY = 0;

    while (std::getline(std::cin, lineStr)) {
        if (lineStr.empty()) continue;

        std::stringstream ss(lineStr);
        Line l;
        char comma;
        std::string arrow;

        if (!(ss >> l.x1 >> comma >> l.y1 >> arrow >> l.x2 >> comma >> l.y2)) continue;

        lines.push_back(l);
        maxX = std::max(maxX, std::max(l.x1, l.x2));
        maxY = std::max(maxY, std::max(l.y1, l.y2));
    }

    std::vector<int> grid((maxX + 1) * (maxY + 1), 0);
    auto idx = [width = maxX + 1](int x, int y) { return y * width + x; };

    for (const auto& l : lines) {
        const int dx = l.x2 - l.x1;
        const int dy = l.y2 - l.y1;
        const int stepX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
        const int stepY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);
        const int length = std::max(std::abs(dx), std::abs(dy));

        for (int i = 0; i <= length; ++i) {
            const int x = l.x1 + stepX * i;
            const int y = l.y1 + stepY * i;
            grid[idx(x, y)]++;
        }
    }

    int overlap = 0;
    for (int count : grid)
        if (count >= 2) overlap++;

    std::cout << overlap << std::endl;
    return 0;
}
