#include <iostream>
#include <string>
#include <vector>
#include <numeric>   // for std::gcd and std::lcm
#include <sstream>
#include <cstdio>    // for std::sscanf

int main() {
    std::vector<int> positions;   // number of positions for each disc
    std::vector<int> start;       // starting position at time=0 for each disc
    std::string line;

    // Read input lines until EOF
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }
        int disc_number = 0;
        int pos = 0;
        int st = 0;
        // Parse lines of the form:
        // Disc #1 has 5 positions; at time=0, it is at position 4.
        if (std::sscanf(
                line.c_str(),
                "Disc #%d has %d positions; at time=0, it is at position %d.",
                &disc_number, &pos, &st) == 3) {
            positions.push_back(pos);
            start.push_back(st);
        } else {
            // If parsing fails, print an error and exit
            std::cerr << "Error: unrecognized input format:\n"
                      << line << std::endl;
            return 1;
        }
    }

    // t will hold the current candidate time;
    // step is the running least common multiple of all processed disc sizes,
    // so we can increment t efficiently without rechecking earlier discs.
    long long t = 0;
    long long step = 1;

    const int n = static_cast<int>(positions.size());
    for (int i = 0; i < n; ++i) {
        // For disc i (0-based in arrays, but 1-based in the problem description)
        // we need (start[i] + t + (i+1)) % positions[i] == 0.
        // We'll increment t by 'step' until this holds, then include this disc
        // in the combined cycle by lcm-ing its period.
        const int disc_index = i + 1;
        const int p = positions[i];
        const int s = start[i];
        // Find the earliest t >= current t that satisfies the constraint
        while ((s + t + disc_index) % p != 0) {
            t += step;
        }
        // Now that disc i is aligned at time t, update step to the lcm
        // of itself and this disc's period, so future increments preserve
        // alignment with all discs up to i.
        step = std::lcm(step, static_cast<long long>(p));
    }

    // Output the result
    std::cout << t << std::endl;
    return 0;
}

