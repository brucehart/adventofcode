#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cstdlib>

int main() {
    // Read the complete input line.
    std::string input;
    std::getline(std::cin, input);

    // Starting position and direction (0: North, 1: East, 2: South, 3: West).
    int x = 0, y = 0;
    int direction = 0; // Initially facing North.

    // Use a string stream to split the input by commas.
    std::istringstream tokenStream(input);
    std::string token;
    while (std::getline(tokenStream, token, ',')) {
        // Remove leading whitespace.
        token.erase(token.begin(),
                    std::find_if(token.begin(), token.end(), [](unsigned char ch) {
                        return !std::isspace(ch);
                    }));
        // Remove trailing whitespace.
        token.erase(std::find_if(token.rbegin(), token.rend(), [](unsigned char ch) {
                        return !std::isspace(ch);
                    }).base(),
                    token.end());
        if (token.empty()) {
            continue;
        }

        // Extract the turn direction ('L' or 'R') and the number of steps.
        char turn = token[0];
        int steps = std::stoi(token.substr(1));

        // Update direction: left turn decreases index (mod 4), right turn increases index.
        if (turn == 'L' || turn == 'l') {
            direction = (direction + 3) % 4; // (direction - 1) mod 4
        } else if (turn == 'R' || turn == 'r') {
            direction = (direction + 1) % 4;
        }

        // Move in the current direction.
        switch (direction) {
            case 0: // North
                y += steps;
                break;
            case 1: // East
                x += steps;
                break;
            case 2: // South
                y -= steps;
                break;
            case 3: // West
                x -= steps;
                break;
        }
    }

    // Compute the Manhattan distance from the starting point.
    int distance = std::abs(x) + std::abs(y);
    std::cout << distance << std::endl;
    return 0;
}

