#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main() {
    std::string line;
    if (!std::getline(std::cin, line)) return 0;

    std::vector<int> positions;
    std::stringstream ss(line);
    std::string value;
    while (std::getline(ss, value, ',')) positions.push_back(std::stoi(value));
    if (positions.empty()) return 0;

    std::sort(positions.begin(), positions.end());
    const int median = positions[positions.size() / 2];

    long long fuel = 0;
    for (int pos : positions) fuel += std::llabs(pos - median);

    std::cout << fuel << std::endl;
    return 0;
}
