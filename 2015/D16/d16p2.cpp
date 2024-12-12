#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <algorithm>

// Function to trim leading and trailing whitespace from a string
std::string trim(const std::string& str) {
    const std::string whitespace = " \t";
    const size_t start = str.find_first_not_of(whitespace);
    if (start == std::string::npos) return ""; // All whitespace
    const size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

int main() {
    // Define the MFCSAM output as a map of property to expected value
    const std::unordered_map<std::string, int> mfcsam = {
        {"children", 3},
        {"cats", 7},
        {"samoyeds", 2},
        {"pomeranians", 3},
        {"akitas", 0},
        {"vizslas", 0},
        {"goldfish", 5},
        {"trees", 3},
        {"cars", 2},
        {"perfumes", 1}
    };

    std::string line;
    while (std::getline(std::cin, line)) {
        // Example line: "Sue 1: cars: 9, akitas: 3, goldfish: 0"

        // Find the position of the first colon to extract Sue number
        size_t colon_pos = line.find(':');
        if (colon_pos == std::string::npos) continue; // Invalid line format

        // Extract "Sue X" part
        std::string sue_part = line.substr(0, colon_pos);
        std::istringstream sue_stream(sue_part);
        std::string sue_label;
        int sue_number;
        sue_stream >> sue_label >> sue_number; // Extract "Sue" and the number

        // Extract the properties part
        std::string properties_str = line.substr(colon_pos + 1);
        std::istringstream prop_stream(properties_str);
        std::string property;
        bool match = true;

        // Process each property separated by ", "
        while (std::getline(prop_stream, property, ',')) {
            // Trim whitespace
            property = trim(property);

            // Split property into key and value
            size_t sep_pos = property.find(':');
            if (sep_pos == std::string::npos) {
                match = false; // Invalid property format
                break;
            }
            std::string key = trim(property.substr(0, sep_pos));
            int value = std::stoi(trim(property.substr(sep_pos + 1)));

            // Check if the property matches the MFCSAM output based on updated rules
            auto it = mfcsam.find(key);
            if (it == mfcsam.end()) {
                // Property not relevant, skip
                continue;
            }

            if (key == "cats" || key == "trees") {
                // For "cats" and "trees", the actual count should be greater than the MFCSAM value
                if (value <= it->second) {
                    match = false;
                    break;
                }
            }
            else if (key == "pomeranians" || key == "goldfish") {
                // For "pomeranians" and "goldfish", the actual count should be less than the MFCSAM value
                if (value >= it->second) {
                    match = false;
                    break;
                }
            }
            else {
                // For other properties, the actual count should exactly match the MFCSAM value
                if (value != it->second) {
                    match = false;
                    break;
                }
            }
        }

        // If all properties match the criteria, output the Sue number and terminate
        if (match) {
            std::cout << sue_number << std::endl;
            return 0;
        }
    }

    // If no matching Sue is found
    std::cerr << "No matching Sue found." << std::endl;
    return 1;
}

