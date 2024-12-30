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

    // Define all eight adjacent directions
    std::vector<std::pair<int, int>> adjacents = {
        {-1, -1}, {0, -1}, {1, -1},
        {-1,  0},          {1,  0},
        {-1,  1}, {0,  1}, {1,  1}
    };

    // Initialize the priority list of directions: N, S, W, E
    std::vector<Direction> directions = {
        {"N", 0, -1, { {0, -1}, {1, -1}, {-1, -1} }},
        {"S", 0, 1,  { {0, 1},  {1, 1},  {-1, 1}  }},
        {"W", -1,0,  { {-1, 0}, {-1, -1}, {-1, 1} }},
        {"E", 1, 0,  { {1, 0},  {1, -1},  {1, 1}  }}
    };

    // Number of simulation rounds
    const int total_rounds = 10;

    for(int round = 0; round < total_rounds; ++round) {
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

        // Process all proposals and execute valid moves
        for(const auto& [dest, sources] : proposals) {
            if(sources.size() == 1) { // Only move if the destination is unique
                Position src = sources[0];
                new_elves.erase(src);   // Remove the source position
                new_elves.insert(dest); // Add the destination position
            }
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

    // Determine the bounding rectangle that contains all Elves
    int min_x = std::numeric_limits<int>::max();
    int max_x = std::numeric_limits<int>::min();
    int min_y = std::numeric_limits<int>::max();
    int max_y = std::numeric_limits<int>::min();

    for(const auto& elf : elves) {
        min_x = std::min(min_x, elf.x);
        max_x = std::max(max_x, elf.x);
        min_y = std::min(min_y, elf.y);
        max_y = std::max(max_y, elf.y);
    }

    // Calculate the area of the bounding rectangle
    long long area = static_cast<long long>(max_x - min_x + 1) * static_cast<long long>(max_y - min_y + 1);

    // Calculate the number of empty ground tiles
    long long empty_ground = area - static_cast<long long>(elves.size());

    // Output the result
    std::cout << empty_ground << std::endl;

    return 0;
}

