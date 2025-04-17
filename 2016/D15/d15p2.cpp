#include <iostream>
#include <string>
#include <vector>
#include <numeric>   // for std::gcd and std::lcm
#include <sstream>
#include <cstdio>    // for std::sscanf

int main() {
    std::vector<int> positions;
    std::vector<int> start;
    std::string line;

    // Read the initial discs configuration from stdin
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }
        int disc_number = 0;
        int pos = 0;
        int st = 0;
        if (std::sscanf(
                line.c_str(),
                "Disc #%d has %d positions; at time=0, it is at position %d.",
                &disc_number, &pos, &st) == 3) {
            positions.push_back(pos);
            start.push_back(st);
        } else {
            std::cerr << "Error: unrecognized input format:\n"
                      << line << std::endl;
            return 1;
        }
    }

    // Append the new disc for Part Two: 11 positions, starting at 0
    positions.push_back(11);
    start.push_back(0);

    // Use the Chinese Remainder-like incremental search:
    long long t = 0;
    long long step = 1;
    const int n = static_cast<int>(positions.size());

    for (int i = 0; i < n; ++i) {
        int disc_index = i + 1;          // 1-based index in problem statement
        int p = positions[i];
        int s = start[i];
        // Find the smallest t (>= current t) such that
        // (s + t + disc_index) % p == 0
        while ((s + t + disc_index) % p != 0) {
            t += step;
        }
        // Incorporate this disc's period into the step via lcm
        step = std::lcm(step, static_cast<long long>(p));
    }

    std::cout << t << std::endl;
    return 0;
}

