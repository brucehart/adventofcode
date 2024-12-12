#include <algorithm>
#include <climits>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>

// Function to parse each input line and update the happiness map
void parse_input(std::map<std::string, std::map<std::string, int>>& happiness_map, const std::string& line) {
    // Example line:
    // "Alice would gain 54 happiness units by sitting next to Bob."
    std::string person, gain_or_lose, neighbor;
    int happiness;

    std::istringstream iss(line);
    std::string word;

    // Extract the first word (person)
    iss >> person;

    // Skip the word "would"
    iss >> word; // "would"

    // Extract "gain" or "lose"
    iss >> gain_or_lose;

    // Extract the happiness value
    iss >> word;
    try {
        happiness = std::stoi(word);
    } catch (const std::invalid_argument& e) {
        // Handle cases where stoi fails
        std::cerr << "Invalid number in line: " << line << "\n";
        throw;
    }

    // If the action is "lose", negate the happiness value
    if (gain_or_lose == "lose") {
        happiness = -happiness;
    }

    // Skip the words "happiness units by sitting next to"
    iss >> word >> word >> word >> word >> word >> word;

    // Extract the neighbor's name, removing the trailing period
    iss >> neighbor;
    if (!neighbor.empty() && neighbor.back() == '.') {
        neighbor.pop_back();
    }

    // Update the happiness map
    happiness_map[person][neighbor] = happiness;
}

int main() {
    // Map to store happiness changes: happiness_map[Person][Neighbor] = happiness_change
    std::map<std::string, std::map<std::string, int>> happiness_map;
    std::string line;

    // Read input from stdin
    while (std::getline(std::cin, line)) {
        if (!line.empty()) { // Ensure the line is not empty
            parse_input(happiness_map, line);
        }
    }

    // Extract all unique guests
    std::vector<std::string> guests;
    for (const auto& pair : happiness_map) {
        guests.push_back(pair.first);
    }

    // *** Part Two: Add Yourself to the Seating Arrangement ***
    std::string you = "You";

    // Add 'You' to the happiness_map with 0 happiness change with everyone
    for (const auto& guest : guests) {
        happiness_map[you][guest] = 0;
        happiness_map[guest][you] = 0;
    }

    // Add 'You' to the guests list
    guests.push_back(you);

    if (guests.empty()) {
        std::cout << "0" << std::endl;
        return 0;
    }

    // To avoid counting circular permutations multiple times, fix the first guest
    std::string fixed_guest = guests[0];
    std::vector<std::string> other_guests(guests.begin() + 1, guests.end());

    // Sort the other guests to ensure all permutations are generated
    std::sort(other_guests.begin(), other_guests.end());

    // Initialize maximum happiness
    long long max_happiness = LLONG_MIN;

    // Generate all possible permutations of the other guests
    do {
        // Create a complete seating arrangement starting with the fixed guest
        std::vector<std::string> seating = {fixed_guest};
        seating.insert(seating.end(), other_guests.begin(), other_guests.end());

        long long current_happiness = 0;
        size_t num_guests = seating.size();

        // Calculate total happiness for this arrangement
        for (size_t i = 0; i < num_guests; ++i) {
            const std::string& current = seating[i];
            const std::string& next = seating[(i + 1) % num_guests]; // Wrap around for circular table

            // Add happiness changes for both neighbors
            current_happiness += happiness_map[current].at(next);
            current_happiness += happiness_map[next].at(current);
        }

        // Update maximum happiness if current is better
        if (current_happiness > max_happiness) {
            max_happiness = current_happiness;
        }

    } while (std::next_permutation(other_guests.begin(), other_guests.end()));

    // Output the maximum total happiness
    std::cout << max_happiness << std::endl;

    return 0;
}

