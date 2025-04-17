#include <algorithm>
#include <iostream>
#include <string>

// Generate data using the modified dragon curve until it is at least 'length' long
std::string generate_data(const std::string& initial, std::size_t length) {
    std::string data = initial;
    // Keep growing until we have enough data
    while (data.size() < length) {
        std::string b = data;
        // Reverse b
        std::reverse(b.begin(), b.end());
        // Invert bits in b: '0' -> '1', '1' -> '0'
        std::transform(b.begin(), b.end(), b.begin(),
                       [](char c) { return c == '0' ? '1' : '0'; });
        // Append '0' and then b to a
        data.push_back('0');
        data += b;
    }
    // Truncate to exact length if overshot
    data.resize(length);
    return data;
}

// Compute the checksum by iteratively processing non-overlapping pairs
std::string compute_checksum(const std::string& data) {
    std::string checksum = data;
    // Repeat while length is even
    while ((checksum.size() % 2) == 0) {
        std::string next;
        next.reserve(checksum.size() / 2);
        for (std::size_t i = 0; i + 1 < checksum.size(); i += 2) {
            // Same bits -> '1', else '0'
            next.push_back(checksum[i] == checksum[i + 1] ? '1' : '0');
        }
        checksum.swap(next);
    }
    return checksum;
}

int main() {
    // Read initial state from stdin
    std::string initial_state;
    if (!(std::cin >> initial_state)) {
        return 1; // No input
    }

    const std::size_t disk_length = 272;

    // Generate the disk data
    std::string data = generate_data(initial_state, disk_length);
    // Compute its checksum
    std::string checksum = compute_checksum(data);

    // Output result
    std::cout << checksum << std::endl;
    return 0;
}

