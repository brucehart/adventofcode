#include <algorithm>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

static inline long long triangular(long long n) { return n * (n + 1) / 2; }

int main() {
    std::string line;
    if (!std::getline(std::cin, line)) return 0;

    std::vector<int> positions;
    std::stringstream ss(line);
    std::string value;
    int minPos = std::numeric_limits<int>::max();
    int maxPos = std::numeric_limits<int>::min();

    while (std::getline(ss, value, ',')) {
        int pos = std::stoi(value);
        positions.push_back(pos);
        minPos = std::min(minPos, pos);
        maxPos = std::max(maxPos, pos);
    }
    if (positions.empty()) return 0;

    long long bestFuel = std::numeric_limits<long long>::max();
    for (int target = minPos; target <= maxPos; ++target) {
        long long fuel = 0;
        for (int pos : positions) {
            fuel += triangular(std::llabs(static_cast<long long>(pos) - target));
            if (fuel >= bestFuel) break;  // early exit if already worse
        }
        bestFuel = std::min(bestFuel, fuel);
    }

    std::cout << bestFuel << std::endl;
    return 0;
}
