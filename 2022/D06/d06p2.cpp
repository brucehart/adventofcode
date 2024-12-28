#include <iostream>
#include <string>
#include <unordered_set>

// Function to determine if all characters in the substring are unique
bool all_unique(const std::string& window) {
    std::unordered_set<char> chars(window.begin(), window.end());
    return chars.size() == window.size();
}

int main() {
    std::string datastream;
    std::string line;

    // Read the datastream from standard input
    // If the datastream spans multiple lines, concatenate them
    while (std::getline(std::cin, line)) {
        datastream += line;
    }

    // Define the size of the start-of-message marker
    const size_t marker_size = 14;

    // Length of the datastream
    size_t length = datastream.size();

    // Iterate through the datastream starting from the (marker_size - 1)-th character
    for (size_t i = marker_size - 1; i < length; ++i) {
        // Extract the current window of 'marker_size' characters ending at position 'i'
        std::string window = datastream.substr(i - (marker_size - 1), marker_size);

        // Check if all characters in the window are unique
        if (all_unique(window)) {
            // Output the position (1-based index) and terminate
            std::cout << (i + 1) << std::endl;
            return 0;
        }
    }

    // If no such marker is found, output an appropriate message
    std::cout << "No start-of-message marker found." << std::endl;
    return 0;
}

