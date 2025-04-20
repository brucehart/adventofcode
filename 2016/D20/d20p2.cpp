#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>

int main() {
    std::vector<std::pair<uint64_t, uint64_t>> blockedRanges;
    std::string line;

    // Read "start-end" ranges from stdin
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }
        auto dashPos = line.find('-');
        uint64_t start = std::stoull(line.substr(0, dashPos));
        uint64_t end   = std::stoull(line.substr(dashPos + 1));
        blockedRanges.emplace_back(start, end);
    }

    // If no ranges, all IPs [0..2^32-1] are allowed
    const uint64_t MAX_IP = 4294967295ULL;
    if (blockedRanges.empty()) {
        // Total count = MAX_IP + 1
        std::cout << (MAX_IP + 1) << std::endl;
        return 0;
    }

    // Sort ranges by start ascending
    std::sort(blockedRanges.begin(), blockedRanges.end(),
              [](auto const& a, auto const& b) {
                  return a.first < b.first;
              });

    uint64_t nextCheck = 0;    // Next IP to check for being allowed
    uint64_t allowedCount = 0; // Total allowed IPs

    for (auto const& range : blockedRanges) {
        // If there's a gap before this range,
        // all IPs from nextCheck to range.first - 1 are allowed
        if (range.first > nextCheck) {
            allowedCount += (range.first - nextCheck);
            nextCheck = range.second + 1;
        }
        else {
            // Overlaps or adjacent: extend nextCheck if this range goes further
            uint64_t candidate = range.second + 1;
            if (candidate > nextCheck) {
                nextCheck = candidate;
            }
        }
        // If we've already passed the max IP, no more allowed
        if (nextCheck > MAX_IP) {
            break;
        }
    }

    // Any remaining IPs after the last blocked range
    if (nextCheck <= MAX_IP) {
        allowedCount += (MAX_IP - nextCheck + 1);
    }

    std::cout << allowedCount << std::endl;
    return 0;
}

