#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct FreshRange {
    long long start;
    long long stop;
};

std::vector<FreshRange> ranges;

int main() {
    long long count = 0;
    std::string line;
    bool readingRanges = true;
    std::vector<long long> values;

    // Read all input, using the blank line as the separator between ranges and values.
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            readingRanges = false;
            continue;
        }

        if (readingRanges) {
            const auto dashPos = line.find('-');
            if (dashPos == std::string::npos) {
                continue;
            }

            const long long start = std::stoll(line.substr(0, dashPos));
            const long long stop = std::stoll(line.substr(dashPos + 1));
            ranges.push_back({start, stop});
        } else {
            values.push_back(std::stoll(line));
        }
    }

    std::sort(ranges.begin(), ranges.end(), [](const FreshRange& a, const FreshRange& b) {
        if (a.start == b.start) {
            return a.stop < b.stop;
        }
        return a.start < b.start;
    });

    // Merge overlapping ranges so binary search can look at a single candidate range.
    std::vector<FreshRange> merged;
    merged.reserve(ranges.size());
    for (const auto& r : ranges) {
        if (merged.empty() || r.start > merged.back().stop) {
            merged.push_back(r);
        } else {
            merged.back().stop = std::max(merged.back().stop, r.stop);
        }
    }

    for (const long long value : values) {
        const auto it = std::upper_bound(
            merged.begin(),
            merged.end(),
            value,
            [](long long val, const FreshRange& r) { return val < r.start; });

        if (it == merged.begin()) {
            continue;
        }

        const auto& candidate = *std::prev(it);
        if (value >= candidate.start && value <= candidate.stop) {
            ++count;
        }
    }

    std::cout << count << std::endl;
}
