#include <iostream>
#include <string>
#include <sstream>

// Function to parse a range string "start-end" into a pair of integers
std::pair<int, int> parse_range(const std::string& range_str) {
    std::pair<int, int> range;
    size_t dash_pos = range_str.find('-');
    if (dash_pos != std::string::npos) {
        range.first = std::stoi(range_str.substr(0, dash_pos));
        range.second = std::stoi(range_str.substr(dash_pos + 1));
    }
    return range;
}

int main() {
    std::string line;
    int total_contained_pairs = 0;

    // Read each line from standard input until EOF
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue; // Skip empty lines if any
        }

        std::stringstream ss(line);
        std::string first_range_str, second_range_str;

        // Split the line by comma to get the two ranges
        if (std::getline(ss, first_range_str, ',') && std::getline(ss, second_range_str)) {
            // Parse both ranges
            std::pair<int, int> range1 = parse_range(first_range_str);
            std::pair<int, int> range2 = parse_range(second_range_str);

            // Check if range1 fully contains range2 or vice versa
            bool range1_contains_range2 = (range1.first <= range2.first) && (range1.second >= range2.second);
            bool range2_contains_range1 = (range2.first <= range1.first) && (range2.second >= range1.second);

            if (range1_contains_range2 || range2_contains_range1) {
                ++total_contained_pairs;
            }
        }
    }

    // Output the final count of contained pairs followed by a newline
    std::cout << total_contained_pairs << std::endl;

    return 0;
}

