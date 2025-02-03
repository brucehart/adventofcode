#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <algorithm>

int main() {
    // Read all lines from standard input into a vector.
    std::vector<std::string> messages;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (!line.empty()) {
            messages.push_back(line);
        }
    }
    
    // If no messages were read, output an empty line.
    if (messages.empty()) {
        std::cout << std::endl;
        return 0;
    }
    
    // All messages are assumed to be of equal length.
    const std::size_t msgLength = messages[0].size();
    
    // Create a frequency table for each column.
    // Each column holds counts for characters 'a' to 'z'.
    std::vector<std::array<int, 26>> frequency(msgLength);
    for (auto& col : frequency) {
        col.fill(0);
    }
    
    // Update the frequency counts for each character in every column.
    for (const auto& msg : messages) {
        for (std::size_t i = 0; i < msgLength; ++i) {
            // Assuming characters are lowercase letters.
            ++frequency[i][msg[i] - 'a'];
        }
    }
    
    // Build the error-corrected message by choosing the most frequent character in each column.
    std::string result;
    result.reserve(msgLength);
    for (std::size_t i = 0; i < msgLength; ++i) {
        // Use std::max_element to find the highest frequency in the column.
        auto maxIt = std::max_element(frequency[i].begin(), frequency[i].end());
        // Convert the index back to the corresponding character.
        char ch = 'a' + static_cast<int>(maxIt - frequency[i].begin());
        result.push_back(ch);
    }
    
    // Output the result followed by a newline.
    std::cout << result << std::endl;
    
    return 0;
}

