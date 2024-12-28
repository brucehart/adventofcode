#include <iostream>
#include <unordered_set>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

// Struct to represent a position on the grid
struct Position {
    int x;
    int y;

    // Overload equality operator for Position
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

// Custom hash function for Position to be used in unordered_set
struct PositionHash {
    std::size_t operator()(const Position& pos) const {
        // Combine hashes of x and y using a prime multiplier to reduce collisions
        return std::hash<int>()(pos.x) * 31 + std::hash<int>()(pos.y);
    }
};

// Function to check if two positions are adjacent (including diagonally)
bool is_adjacent(const Position& a, const Position& b) {
    return std::abs(a.x - b.x) <= 1 && std::abs(a.y - b.y) <= 1;
}

// Function to move a knot towards the target by one step
void move_knot(Position& knot, const Position& target) {
    int dx = target.x - knot.x;
    int dy = target.y - knot.y;

    // Normalize the difference to get the direction (-1, 0, 1)
    if (dx != 0) {
        knot.x += (dx > 0) ? 1 : -1;
    }
    if (dy != 0) {
        knot.y += (dy > 0) ? 1 : -1;
    }
}

int main() {
    // Initialize a rope with 10 knots, all starting at position (0, 0)
    std::vector<Position> knots(10, Position{0, 0});

    // Unordered set to store unique positions visited by the last knot (knot 9)
    std::unordered_set<Position, PositionHash> visited;
    visited.emplace(knots[9]); // Starting position

    std::string line;
    // Read input line by line
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue; // Skip empty lines

        std::stringstream ss(line);
        char direction;
        int steps;
        ss >> direction >> steps;

        // Determine movement delta based on direction
        int dx = 0, dy = 0;
        switch (direction) {
            case 'R': dx = 1; break;
            case 'L': dx = -1; break;
            case 'U': dy = 1; break;
            case 'D': dy = -1; break;
            default:
                // Invalid direction, skip this instruction
                continue;
        }

        // Move step by step
        for (int i = 0; i < steps; ++i) {
            // Move the head (knot 0)
            knots[0].x += dx;
            knots[0].y += dy;

            // Iterate through each subsequent knot to update its position
            for (size_t k = 1; k < knots.size(); ++k) {
                // If the current knot is not adjacent to the previous knot, move it
                if (!is_adjacent(knots[k], knots[k - 1])) {
                    move_knot(knots[k], knots[k - 1]);
                }
                // If adjacent, no movement is needed
            }

            // After moving all knots, record the position of the last knot
            visited.emplace(knots[9]);
        }
    }

    // Output the number of unique positions visited by the tail (knot 9)
    std::cout << visited.size() << std::endl;

    return 0;
}

