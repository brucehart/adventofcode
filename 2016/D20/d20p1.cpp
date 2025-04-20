#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>

int main() {
    std::vector<std::pair<uint64_t, uint64_t>> blockedRanges;
    std::string line;

    // Read and parse each "start-end" line from stdin
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }
        std::size_t dashPos = line.find('-');
        uint64_t start = std::stoull(line.substr(0, dashPos));
        uint64_t end   = std::stoull(line.substr(dashPos + 1));
        blockedRanges.emplace_back(start, end);
    }

    // No input => no blocked IPs, lowest allowed is 0
    if (blockedRanges.empty()) {
        std::cout << 0 << std::endl;
        return 0;
    }

    // Sort ranges by their start value
    std::sort(blockedRanges.begin(), blockedRanges.end(),
              [](auto const& a, auto const& b) {
                  return a.first < b.first;
              });

    // Track the first IP not yet covered by blockedRanges
    uint64_t nextAllowed = 0;

    for (auto const& range : blockedRanges) {
        // If there's a gap before this range, that's the lowest allowed IP
        if (range.first > nextAllowed) {
            std::cout << nextAllowed << std::endl;
            return 0;
        }

        // Otherwise, extend our coverage if this range pushes it further.
        // +1 because ranges are inclusive.
        uint64_t candidate = range.second + 1;
        if (candidate > nextAllowed) {
            nextAllowed = candidate;
        }
    }

    // If we've merged through all ranges without finding a gap,
    // then the first allowed IP is just after the last blocked one.
    // Ensure it fits within 0..2^32-1.
    const uint64_t MAX_IP = 4294967295ULL;
    if (nextAllowed <= MAX_IP) {
        std::cout << nextAllowed << std::endl;
    }

    return 0;
}

