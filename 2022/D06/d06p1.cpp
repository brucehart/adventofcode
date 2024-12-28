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
    
    // Read the datastream from standard input
    // Assuming the datastream is provided as a single line
    std::getline(std::cin, datastream);

    // Length of the datastream
    size_t length = datastream.size();

    // Iterate through the datastream starting from the 4th character
    for (size_t i = 3; i < length; ++i) {
        // Extract the last four characters ending at the current position
        std::string window = datastream.substr(i - 3, 4);

        // Check if all four characters are unique
        if (all_unique(window)) {
            // Output the position (1-based index) and terminate
            std::cout << (i + 1) << std::endl;
            return 0;
        }
    }

    // If no such marker is found, output 0 or an appropriate message
    std::cout << "No start-of-packet marker found." << std::endl;
    return 0;
}

