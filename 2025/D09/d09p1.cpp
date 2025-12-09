#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Point {
    long long x;
    long long y;
};

int main() {
    std::vector<Point> points;
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }
        std::replace(line.begin(), line.end(), ',', ' ');
        std::stringstream ss(line);
        Point p{};
        if (ss >> p.x >> p.y) {
            points.push_back(p);
        }
    }

    if (points.size() < 2) {
        std::cout << 0 << '\n';
        return 0;
    }

    long long best = 0;
    for (std::size_t i = 0; i + 1 < points.size(); ++i) {
        for (std::size_t j = i + 1; j < points.size(); ++j) {
            const long long width = std::llabs(points[i].x - points[j].x) + 1;
            const long long height = std::llabs(points[i].y - points[j].y) + 1;
            const long long area = width * height;
            if (area > best) {
                best = area;
            }
        }
    }

    std::cout << best << std::endl;
    return 0;
}
