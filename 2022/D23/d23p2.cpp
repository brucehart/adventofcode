#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <limits>

// Struct to represent a position with x and y coordinates
struct Position {
    int x;
    int y;

    // Equality operator to compare two positions
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

// Custom hash function for Position to be used in unordered containers
struct PositionHash {
    std::size_t operator()(const Position& pos) const {
        // Combine hashes of x and y using a prime multiplier
        return std::hash<int>()(pos.x) * 1000003 + std::hash<int>()(pos.y);
    }
};

// Struct to represent a direction with movement delta and positions to check
struct Direction {
    std::string name; // Direction name (e.g., "N", "S")
    int dx;           // Change in x for movement
    int dy;           // Change in y for movement
    std::vector<std::pair<int, int>> checks; // Relative positions to check for validity
};

int main() {
    // Initialize a set to store positions of all Elves
    std::unordered_set<Position, PositionHash> elves;

    // Read input grid from stdin
    std::string line;
    int y = 0;
    while (std::getline(std::cin, line)) {
        for(int x = 0; x < static_cast<int>(line.size()); ++x) {
            if(line[x] == '#') {
                elves.insert(Position{ x, y });
            }
        }
        y++;
    }

    // Define all eight adjacent directions (N, NE, E, SE, S, SW, W, NW)
    std::vector<std::pair<int, int>> adjacents = {
        {0, -1},  // N
        {1, -1},  // NE
        {1, 0},   // E
        {1, 1},   // SE
        {0, 1},   // S
        {-1, 1},  // SW
        {-1, 0},  // W
        {-1, -1}  // NW
    };

    // Initialize the priority list of directions: N, S, W, E
    std::vector<Direction> directions = {
        {"N", 0, -1, { {0, -1}, {1, -1}, {-1, -1} }},
        {"S", 0, 1,  { {0, 1},  {1, 1},  {-1, 1}  }},
        {"W", -1,0,  { {-1, 0}, {-1, -1}, {-1, 1} }},
        {"E", 1, 0,  { {1, 0},  {1, -1},  {1, 1}  }}
    };

    int round = 0; // Initialize round counter

    while (true) {
        round++; // Increment round number

        // Map to store proposed moves: destination -> list of source positions
        std::unordered_map<Position, std::vector<Position>, PositionHash> proposals;

        // Iterate over each Elf to determine their move proposals
        for(const auto& elf : elves) {
            bool has_adjacent = false;

            // Check all adjacent positions for other Elves
            for(const auto& delta : adjacents) {
                Position adjacent_pos{ elf.x + delta.first, elf.y + delta.second };
                if(elves.find(adjacent_pos) != elves.end()) {
                    has_adjacent = true;
                    break;
                }
            }

            // If no adjacent Elves, do not propose any move
            if(!has_adjacent) {
                continue;
            }

            // Consider each direction in the current priority order
            for(const auto& dir : directions) {
                bool can_move = true;

                // Check the three positions required for the current direction
                for(const auto& check_delta : dir.checks) {
                    Position check_pos{ elf.x + check_delta.first, elf.y + check_delta.second };
                    if(elves.find(check_pos) != elves.end()) {
                        can_move = false;
                        break;
                    }
                }

                // If the direction is valid, propose to move
                if(can_move) {
                    Position dest{ elf.x + dir.dx, elf.y + dir.dy };
                    proposals[dest].push_back(elf);
                    break; // Only propose the first valid direction
                }
            }
        }

        // Set to store updated positions after movement
        std::unordered_set<Position, PositionHash> new_elves = elves;
        int moved_elves = 0; // Counter for moved Elves

        // Process all proposals and execute valid moves
        for(const auto& [dest, sources] : proposals) {
            if(sources.size() == 1) { // Only move if the destination is unique
                Position src = sources[0];
                // Erase the source position
                new_elves.erase(src);
                // Insert the destination position
                new_elves.insert(dest);
                moved_elves++; // Increment moved Elves count
            }
        }

        // If no Elves moved in this round, terminate and output the round number
        if(moved_elves == 0) {
            std::cout << round << std::endl;
            return 0;
        }

        // Update the Elves' positions
        elves = std::move(new_elves);

        // Rotate the direction priority: move the first direction to the end
        if(!directions.empty()) {
            Direction first_dir = directions.front();
            directions.erase(directions.begin());
            directions.push_back(first_dir);
        }
    }

    return 0;
}

