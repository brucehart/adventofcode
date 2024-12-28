#include <iostream>
#include <string>
#include <bitset>

// Function to calculate the priority of a given character
int get_priority(char c) {
    if (c >= 'a' && c <= 'z') {
        return static_cast<int>(c - 'a' + 1);
    } else if (c >= 'A' && c <= 'Z') {
        return static_cast<int>(c - 'A' + 27);
    }
    // If the character is not a valid item type, return 0
    return 0;
}

int main() {
    std::string line1, line2, line3;
    long long total_priority = 0;

    // Read lines in groups of three until EOF
    while (std::getline(std::cin, line1) &&
           std::getline(std::cin, line2) &&
           std::getline(std::cin, line3)) {
        
        // Create bitsets to represent the presence of characters in each rucksack
        std::bitset<256> bits1, bits2, bits3;
        for (char c : line1) {
            bits1.set(static_cast<unsigned char>(c));
        }
        for (char c : line2) {
            bits2.set(static_cast<unsigned char>(c));
        }
        for (char c : line3) {
            bits3.set(static_cast<unsigned char>(c));
        }

        // Find the common characters by performing bitwise AND
        std::bitset<256> common = bits1 & bits2 & bits3;

        // Identify the common character
        char common_char = '\0';
        for (int i = 0; i < 256; ++i) {
            if (common.test(i)) {
                common_char = static_cast<char>(i);
                break; // Since there's exactly one common item, we can break early
            }
        }

        // Add the priority of the common character to the total
        if (common_char != '\0') {
            total_priority += get_priority(common_char);
        }
    }

    // Output the final sum of priorities followed by a newline
    std::cout << total_priority << std::endl;

    return 0;
}

