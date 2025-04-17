#include <algorithm>
#include <iostream>
#include <string>

// Generate data using the modified dragon curve until it is at least 'length' long
std::string generate_data(const std::string& initial, std::size_t length) {
    std::string data = initial;
    data.reserve(length);  // avoid repeated reallocations
    while (data.size() < length) {
        std::string b = data;
        std::reverse(b.begin(), b.end());
        std::transform(b.begin(), b.end(), b.begin(),
                       [](char c) { return c == '0' ? '1' : '0'; });
        data.push_back('0');
        data += b;
    }
    data.resize(length);
    return data;
}

// Compute the checksum by iteratively processing non-overlapping pairs
std::string compute_checksum(const std::string& data) {
    std::string checksum = data;
    while ((checksum.size() % 2) == 0) {
        std::string next;
        next.reserve(checksum.size() / 2);
        for (std::size_t i = 0; i + 1 < checksum.size(); i += 2) {
            next.push_back(checksum[i] == checksum[i + 1] ? '1' : '0');
        }
        checksum.swap(next);
    }
    return checksum;
}

int main() {
    std::string initial_state;
    if (!(std::cin >> initial_state)) {
        return 1; // No input provided
    }

    const std::size_t disk_length = 35651584;

    std::string data = generate_data(initial_state, disk_length);
    std::string checksum = compute_checksum(data);

    std::cout << checksum << std::endl;
    return 0;
}

