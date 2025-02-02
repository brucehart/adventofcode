#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <cctype>
#include <cstdlib>

// Custom hash for std::pair<int, int> to be used in an unordered_set.
struct PairHash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        std::size_t h1 = std::hash<int>{}(p.first);
        std::size_t h2 = std::hash<int>{}(p.second);
        return h1 ^ (h2 << 1);  // Combine hashes.
    }
};

int main() {
    // Read the entire input line containing the instructions.
    std::string input;
    std::getline(std::cin, input);

    // Starting position (0, 0) and initial direction (0: North, 1: East, 2: South, 3: West).
    int x = 0, y = 0, direction = 0;

    // Use an unordered_set to track all visited coordinates.
    std::unordered_set<std::pair<int, int>, PairHash> visited;
    visited.insert({0, 0}); // Starting point is visited.

    // Use a string stream to split the input by commas.
    std::istringstream tokenStream(input);
    std::string token;
    while (std::getline(tokenStream, token, ',')) {
        // Trim leading whitespace.
        token.erase(token.begin(), std::find_if(token.begin(), token.end(),
                                                  [](unsigned char ch) { return !std::isspace(ch); }));
        // Trim trailing whitespace.
        token.erase(std::find_if(token.rbegin(), token.rend(),
                                 [](unsigned char ch) { return !std::isspace(ch); }).base(),
                    token.end());
        if (token.empty()) {
            continue;
        }

        // The first character is the turn direction ('L' or 'R'),
        // followed by the number of blocks to move.
        char turn = token[0];
        int steps = std::stoi(token.substr(1));

        // Update direction: turning left decreases direction index, turning right increases it.
        if (turn == 'L' || turn == 'l') {
            direction = (direction + 3) % 4; // equivalent to (direction - 1) mod 4.
        } else if (turn == 'R' || turn == 'r') {
            direction = (direction + 1) % 4;
        }

        // Move one step at a time and check for revisited locations.
        for (int i = 0; i < steps; ++i) {
            switch (direction) {
                case 0: // North
                    ++y;
                    break;
                case 1: // East
                    ++x;
                    break;
                case 2: // South
                    --y;
                    break;
                case 3: // West
                    --x;
                    break;
            }

            // Check if the current location has been visited before.
            std::pair<int, int> currentPos = {x, y};
            if (visited.find(currentPos) != visited.end()) {
                // Output the Manhattan distance and exit.
                int distance = std::abs(x) + std::abs(y);
                std::cout << distance << std::endl;
                return 0;
            }
            visited.insert(currentPos);
        }
    }

    // If no location is visited twice, output the Manhattan distance of the final location.
    int distance = std::abs(x) + std::abs(y);
    std::cout << distance << std::endl;
    return 0;
}

