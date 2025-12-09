#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main() {
    std::string line;
    if (!std::getline(std::cin, line)) return 0;

    std::vector<long long> timers(9, 0);
    std::stringstream ss(line);
    std::string value;
    while (std::getline(ss, value, ',')) timers[std::stoi(value)]++;

    for (int day = 0; day < 80; ++day) {
        const long long births = timers[0];
        for (int t = 0; t < 8; ++t) timers[t] = timers[t + 1];
        timers[6] += births;
        timers[8] = births;
    }

    long long total = 0;
    for (long long count : timers) total += count;

    std::cout << total << std::endl;
    return 0;
}
