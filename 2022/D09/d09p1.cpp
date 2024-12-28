#include <iostream>
#include <unordered_set>
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
        // Combine hashes of x and y using a common hashing technique
        // to reduce collisions
        return std::hash<int>()(pos.x) * 31 + std::hash<int>()(pos.y);
    }
};

// Function to check if two positions are adjacent (including diagonally)
bool is_adjacent(const Position& head, const Position& tail) {
    return std::abs(head.x - tail.x) <= 1 && std::abs(head.y - tail.y) <= 1;
}

// Function to move the tail towards the head by one step
void move_tail(Position& tail, const Position& head) {
    // Calculate the difference in coordinates
    int dx = head.x - tail.x;
    int dy = head.y - tail.y;

    // Normalize the difference to get the direction
    if (dx != 0) {
        tail.x += (dx > 0) ? 1 : -1;
    }
    if (dy != 0) {
        tail.y += (dy > 0) ? 1 : -1;
    }
}

int main() {
    // Initialize head and tail positions at (0,0)
    Position head{0, 0};
    Position tail{0, 0};

    // Unordered set to store unique tail positions
    std::unordered_set<Position, PositionHash> visited;
    visited.emplace(tail); // Starting position

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
                // Invalid direction, skip
                continue;
        }

        // Move step by step
        for (int i = 0; i < steps; ++i) {
            // Move head
            head.x += dx;
            head.y += dy;

            // Check if tail needs to move
            if (!is_adjacent(head, tail)) {
                move_tail(tail, head);
                // Add new tail position to visited
                visited.emplace(tail);
            }
            // If tail doesn't move, it's still in the current position, already in set
        }
    }

    // Output the number of unique positions visited by the tail
    std::cout << visited.size() << std::endl;

    return 0;
}

