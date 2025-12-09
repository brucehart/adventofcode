#include <algorithm>
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
        if (l.x1 == l.x2) {
            int start = std::min(l.y1, l.y2);
            int end = std::max(l.y1, l.y2);
            for (int y = start; y <= end; ++y) grid[idx(l.x1, y)]++;
        } else if (l.y1 == l.y2) {
            int start = std::min(l.x1, l.x2);
            int end = std::max(l.x1, l.x2);
            for (int x = start; x <= end; ++x) grid[idx(x, l.y1)]++;
        }
    }

    int overlap = 0;
    for (int count : grid)
        if (count >= 2) overlap++;

    std::cout << overlap << std::endl;
    return 0;
}
