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
    std::string line;
    long long total_priority = 0;

    // Read each line from standard input until EOF
    while (std::getline(std::cin, line)) {
        // Calculate the midpoint to split the rucksack into two compartments
        size_t midpoint = line.size() / 2;
        std::string first_compartment = line.substr(0, midpoint);
        std::string second_compartment = line.substr(midpoint, midpoint);

        // Use a bitset to record the presence of characters in the first compartment
        std::bitset<256> char_set;
        for (char c : first_compartment) {
            char_set.set(static_cast<unsigned char>(c));
        }

        // Find the first character in the second compartment that also exists in the first
        char common_char = '\0';
        for (char c : second_compartment) {
            if (char_set.test(static_cast<unsigned char>(c))) {
                common_char = c;
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

