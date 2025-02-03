#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <limits>

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
    
    // Count the frequency of each character in each column.
    for (const auto& msg : messages) {
        for (std::size_t i = 0; i < msgLength; ++i) {
            // Assuming characters are lowercase letters.
            ++frequency[i][msg[i] - 'a'];
        }
    }
    
    // Build the original message by choosing the least common character (ignoring zeros)
    std::string result;
    result.reserve(msgLength);
    for (std::size_t i = 0; i < msgLength; ++i) {
        int minFreq = std::numeric_limits<int>::max();
        int minIndex = 0;
        // Iterate over each letter in the column.
        for (int j = 0; j < 26; ++j) {
            // Only consider letters that appear at least once.
            if (frequency[i][j] > 0 && frequency[i][j] < minFreq) {
                minFreq = frequency[i][j];
                minIndex = j;
            }
        }
        // Convert the index back to a character.
        result.push_back('a' + minIndex);
    }
    
    // Output the result followed by a newline.
    std::cout << result << std::endl;
    
    return 0;
}

