#include <bits/stdc++.h>

// Define Position as a struct with row and column
struct Position {
    int r; // Row
    int c; // Column

    // Comparator for sorting and storing in ordered containers
    bool operator<(const Position& other) const {
        return std::tie(r, c) < std::tie(other.r, other.c);
    }

    // Equality operator for comparison
    bool operator==(const Position& other) const {
        return r == other.r && c == other.c;
    }
};

// Define Box as a pair of Positions
using Box = std::pair<Position, Position>;

// Comparator for Box to be used in std::set
struct BoxComparator {
    bool operator()(const Box& a, const Box& b) const {
        if (a.first < b.first) return true;
        if (b.first < a.first) return false;
        return a.second < b.second;
    }
};

// Define BoxSet as a set of Boxes with the above comparator
using BoxSet = std::set<Box, BoxComparator>;

// Enum for move status
enum class StatusType { Blocked, Move };

// Struct to hold move status and boxes to move
struct MoveStatus {
    StatusType type;
    BoxSet boxesToMove;

    MoveStatus(StatusType t) : type(t) {}
    MoveStatus(StatusType t, const BoxSet& b) : type(t), boxesToMove(b) {}
};

// Function to split input into grid and directions based on an empty line separator
std::pair<std::vector<std::string>, std::string> splitInput(const std::vector<std::string>& lines) {
    std::vector<std::string> grid;
    std::string directions;
    bool foundEmpty = false;
    for (const auto& line : lines) {
        if (line.empty()) {
            foundEmpty = true;
            continue;
        }
        if (!foundEmpty) {
            grid.push_back(line);
        } else {
            directions += line;
        }
    }
    return {grid, directions};
}

// Function to scan the grid and identify robot, boxes, and obstacles
std::tuple<std::pair<int, int>, Position, BoxSet, BoxSet> scanGrid(const std::vector<std::string>& grid) {
    int height = static_cast<int>(grid.size());
    if (height == 0) {
        throw std::runtime_error("Empty grid provided.");
    }
    int width = 2 * static_cast<int>(grid[0].size());
    Position robot;
    bool robotFound = false;
    BoxSet boxes;
    BoxSet obstacles;

    for (int r = 0; r < height; ++r) {
        const std::string& row = grid[r];
        for (int c = 0; c < static_cast<int>(row.size()); ++c) {
            char cell = row[c];
            if (cell == '@' && !robotFound) {
                robot = Position{r, 2 * c};
                robotFound = true;
            }
            else if (cell == 'O') {
                boxes.emplace(Box{Position{r, 2 * c}, Position{r, 2 * c + 1}});
            }
            else if (cell == '#') {
                obstacles.emplace(Box{Position{r, 2 * c}, Position{r, 2 * c + 1}});
            }
        }
    }

    if (!robotFound) {
        throw std::runtime_error("Robot '@' not found in the grid.");
    }

    return {std::make_pair(height, width), robot, boxes, obstacles};
}

// Function to check if a position overlaps with any in the objects set
bool overlaps(const Position& pos, const BoxSet& objects) {
    return std::any_of(objects.begin(), objects.end(),
                       [&pos](const Box& box) { return pos == box.first || pos == box.second; });
}

// Function to get all overlapping boxes for a given position
BoxSet getOverlap(const Position& pos, const BoxSet& objects) {
    BoxSet overlapsSet;
    std::copy_if(objects.begin(), objects.end(), std::inserter(overlapsSet, overlapsSet.begin()),
                 [&pos](const Box& box) { return pos == box.first || pos == box.second; });
    return overlapsSet;
}

// Function to scan boxes to determine if a move is possible and identify boxes to move
MoveStatus scanBoxes(const Position& robot, const std::pair<int, int>& delta,
                    const BoxSet& boxes, const BoxSet& obstacles) {
    BoxSet workingSet;
    // Calculate the next position the robot intends to move to
    Position nextPos{robot.r + delta.first, robot.c + delta.second};

    // If the next position overlaps with any box, add those boxes to the working set
    if (overlaps(nextPos, boxes)) {
        BoxSet initialOverlap = getOverlap(nextPos, boxes);
        workingSet = initialOverlap;
    }
    else {
        // If the next position is blocked by an obstacle, the move is blocked
        if (overlaps(nextPos, obstacles)) {
            return MoveStatus(StatusType::Blocked);
        }
        else {
            // Move is possible without moving any boxes
            return MoveStatus(StatusType::Move);
        }
    }

    // Iteratively check for cascading box movements
    bool canMove = true;
    while (canMove) {
        BoxSet addedBoxes;
        for (const auto& box : workingSet) {
            // Calculate the new positions if the box is moved
            Position movedPos1{box.first.r + delta.first, box.first.c + delta.second};
            Position movedPos2{box.second.r + delta.first, box.second.c + delta.second};

            // Check for overlaps with other boxes
            if (overlaps(movedPos1, boxes)) {
                BoxSet overlap = getOverlap(movedPos1, boxes);
                addedBoxes.insert(overlap.begin(), overlap.end());
            }
            if (overlaps(movedPos2, boxes)) {
                BoxSet overlap = getOverlap(movedPos2, boxes);
                addedBoxes.insert(overlap.begin(), overlap.end());
            }
        }

        // Remove already processed boxes to avoid infinite loops
        for (const auto& box : workingSet) {
            addedBoxes.erase(box);
        }

        if (addedBoxes.empty()) {
            canMove = false;
        }
        else {
            // Add newly identified boxes to the working set
            size_t previousSize = workingSet.size();
            workingSet.insert(addedBoxes.begin(), addedBoxes.end());
            // If no new boxes were added, stop the loop
            if (workingSet.size() == previousSize) {
                canMove = false;
            }
        }
    }

    // After identifying all boxes to move, check if the destination is blocked by obstacles
    for (const auto& box : workingSet) {
        Position movedPos1{box.first.r + delta.first, box.first.c + delta.second};
        Position movedPos2{box.second.r + delta.first, box.second.c + delta.second};
        if (overlaps(movedPos1, obstacles) || overlaps(movedPos2, obstacles)) {
            return MoveStatus(StatusType::Blocked);
        }
    }

    return MoveStatus(StatusType::Move, workingSet);
}

// Function to apply the move based on MoveStatus
std::pair<Position, BoxSet> applyMove(const Position& robot, const std::pair<int, int>& delta,
                                      const MoveStatus& status, const BoxSet& boxes) {
    if (status.type == StatusType::Blocked) {
        // If move is blocked, robot and boxes remain unchanged
        return {robot, boxes};
    }
    else {
        // Move the robot
        Position newRobot{robot.r + delta.first, robot.c + delta.second};
        BoxSet updatedBoxes;

        // Iterate through all boxes to update their positions if they are in the boxesToMove set
        for (const auto& box : boxes) {
            if (status.boxesToMove.find(box) != status.boxesToMove.end()) {
                // Move the box by delta
                Position newPos1{box.first.r + delta.first, box.first.c + delta.second};
                Position newPos2{box.second.r + delta.first, box.second.c + delta.second};
                updatedBoxes.emplace(Box{newPos1, newPos2});
            }
            else {
                // Keep the box unchanged
                updatedBoxes.emplace(box);
            }
        }

        return {newRobot, updatedBoxes};
    }
}

// Function to move the robot in a given direction
std::pair<Position, BoxSet> moveRobot(char dir, const Position& robot, const BoxSet& boxes,
                                      const BoxSet& obstacles,
                                      const std::unordered_map<char, std::pair<int, int>>& directionsMap) {
    // Retrieve the direction delta from the directions map
    auto it = directionsMap.find(dir);
    if (it == directionsMap.end()) {
        throw std::runtime_error("Unexpected character in input: " + std::string(1, dir));
    }
    const std::pair<int, int>& delta = it->second;

    // Determine the move status by scanning boxes
    MoveStatus status = scanBoxes(robot, delta, boxes, obstacles);

    // Apply the move based on the move status
    return applyMove(robot, delta, status, boxes);
}

// Function to process all moves sequentially
std::pair<Position, BoxSet> processMoves(const std::string& directions, Position robot,
                                         BoxSet boxes, const BoxSet& obstacles,
                                         const std::unordered_map<char, std::pair<int, int>>& directionsMap) {
    for (char dir : directions) {
        std::tie(robot, boxes) = moveRobot(dir, robot, boxes, obstacles, directionsMap);
    }
    return {robot, boxes};
}

// Function to calculate GPS value for a box
int calcGPS(const Box& box) {
    return (100 * box.first.r) + box.first.c;
}

int main() {
    try {
        // Read all lines from stdin
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(std::cin, line)) {
            lines.push_back(line);
        }

        // Split input into grid and directions
        auto input = splitInput(lines);
        const std::vector<std::string>& grid = input.first;
        const std::string& directions = input.second;

        // Scan the grid to get initial positions
        auto gridInfo = scanGrid(grid);
        // std::pair<int, int> gridSize = std::get<0>(gridInfo); // Not used further
        Position robot = std::get<1>(gridInfo);
        BoxSet boxes = std::get<2>(gridInfo);
        BoxSet obstacles = std::get<3>(gridInfo);

        // Define directions map
        const std::unordered_map<char, std::pair<int, int>> directionsMap = {
            {'^', {-1, 0}},
            {'v', {1, 0}},
            {'<', {0, -1}},
            {'>', {0, 1}},
        };

        // Process all moves
        auto finalState = processMoves(directions, robot, boxes, obstacles, directionsMap);
        Position finalRobot = finalState.first;
        BoxSet finalBoxes = finalState.second;

        // Calculate the result by summing GPS values of all boxes
        int result = std::accumulate(finalBoxes.begin(), finalBoxes.end(), 0,
                                     [](int sum, const Box& box) { return sum + calcGPS(box); });

        // Output the result
        std::cout << result << std::endl;
    }
    catch (const std::exception& ex) {
        // Handle any errors by printing the message and exiting with failure
        std::cerr << "Error: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}
