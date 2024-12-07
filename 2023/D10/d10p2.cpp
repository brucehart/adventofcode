#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <utility>
#include <queue>
#include <cmath>

// Custom hash function for pair<int, int> to use in unordered_set
struct pair_hash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

// Structure to hold the path of the loop
struct Path {
    std::vector<std::pair<int, int>> positions;
};

struct Maze {
    // Define directions
    enum DIRECTIONS { NORTH = 0, SOUTH = 1, EAST = 2, WEST = 3 };
    std::vector<int> inverse = { SOUTH, NORTH, WEST, EAST };
    std::vector<std::pair<int, int>> move = {
        {-1, 0},  // NORTH
        {1, 0},   // SOUTH
        {0, 1},   // EAST
        {0, -1}   // WEST
    };

    struct MoveSet {
        int connectA;
        int connectB;
        char mazeChar;
    };

    // Define the connections for each pipe character
    std::vector<MoveSet> mazeMoves = {
        {NORTH, SOUTH, '|'},
        {EAST, WEST, '-'},
        {NORTH, EAST, 'L'},
        {NORTH, WEST, 'J'},
        {SOUTH, EAST, 'F'},
        {SOUTH, WEST, '7'}
    };

    std::vector<std::vector<char>> board;
    std::unordered_set<std::pair<int, int>, pair_hash> visited;

    std::pair<int, int> startPos;

    // Get the number of rows
    int rows() const {
        return static_cast<int>(board.size());
    }

    // Get the number of columns
    int cols() const {
        return (rows() > 0) ? static_cast<int>(board[0].size()) : 0;
    }

    // Safely get the character at (i, j)
    char get(int i, int j) const {
        if (i < 0 || i >= rows() || j < 0 || j >= cols()) {
            return '\0';
        }
        return board[i][j];
    }

    // Insert a row into the board
    void insertRow(const std::string& rowData) {
        std::vector<char> row(rowData.begin(), rowData.end());
        board.push_back(row);
    }

    // Determine the pipe character based on two connected directions
    char getPipeCharForDirections(int dir1, int dir2) const {
        if ((dir1 == NORTH && dir2 == SOUTH) || (dir1 == SOUTH && dir2 == NORTH)) return '|';
        if ((dir1 == EAST && dir2 == WEST) || (dir1 == WEST && dir2 == EAST)) return '-';
        if ((dir1 == NORTH && dir2 == EAST) || (dir1 == EAST && dir2 == NORTH)) return 'L';
        if ((dir1 == NORTH && dir2 == WEST) || (dir1 == WEST && dir2 == NORTH)) return 'J';
        if ((dir1 == SOUTH && dir2 == EAST) || (dir1 == EAST && dir2 == SOUTH)) return 'F';
        if ((dir1 == SOUTH && dir2 == WEST) || (dir1 == WEST && dir2 == SOUTH)) return '7';
        return 'X'; // Undefined or invalid configuration
    }

    // Find 'S', determine its connections, and replace it with the correct pipe character
    bool findStartAndReplaceS() {
        for (int i = 0; i < rows(); ++i) {
            for (int j = 0; j < cols(); ++j) {
                if (get(i, j) == 'S') {
                    startPos = {i, j};
                    
                    std::vector<int> startDirs;
                    // Check all four directions for connections
                    for (int d = 0; d < 4; ++d) {
                        int ni = i + move[d].first;
                        int nj = j + move[d].second;
                        char c = get(ni, nj);
                        if (c == '\0' || c == '.' ) continue;
                        // Determine if the adjacent pipe connects back to 'S'
                        int incDir = inverse[d];
                        bool connected = false;
                        for (const auto& m : mazeMoves) {
                            if (m.mazeChar == c) {
                                if (m.connectA == incDir || m.connectB == incDir) {
                                    connected = true;
                                    break;
                                }
                            }
                        }
                        if (connected) {
                            startDirs.push_back(d);
                        }
                    }

                    // 'S' should have exactly two connections in a loop
                    if (startDirs.size() != 2) {
                        std::cerr << "Error: 'S' does not have exactly two connections.\n";
                        return false;
                    }

                    // Determine the pipe character for 'S' based on its connections
                    char pipeChar = getPipeCharForDirections(startDirs[0], startDirs[1]);
                    if (pipeChar == 'X') {
                        std::cerr << "Error: Undefined pipe configuration for 'S'.\n";
                        return false;
                    }
                    board[i][j] = pipeChar;
                    return true;
                }
            }
        }
        std::cerr << "Error: 'S' not found in the maze.\n";
        return false;
    }

    // Given a pipe character and an incoming direction, determine the outgoing direction
    int getNextDirection(char pipeChar, int incomingDir) const {
        for (const auto &m : mazeMoves) {
            if (m.mazeChar == pipeChar) {
                if (m.connectA == incomingDir) {
                    return m.connectB;
                } else if (m.connectB == incomingDir) {
                    return m.connectA;
                }
            }
        }
        return -1; // No valid connection found
    }

    // Traverse the loop step-by-step and collect the path
    int traverseLoopStepByStep(Path &path) {
        if (!findStartAndReplaceS()) {
            // Failed to find or replace 'S' correctly
            return 0;
        }

        // Initialize path with starting position
        path.positions.emplace_back(startPos);

        // Determine initial direction from the starting position
        std::vector<int> startDirs;
        for (int d = 0; d < 4; ++d) {
            int ni = startPos.first + move[d].first;
            int nj = startPos.second + move[d].second;
            char c = get(ni, nj);
            if (c == '\0' || c == '.' ) continue;
            int incDir = inverse[d];
            int outDir = getNextDirection(c, incDir);
            if (outDir != -1) {
                startDirs.push_back(outDir);
            }
        }

        if (startDirs.size() != 2) {
            std::cerr << "Error: Starting pipe does not have exactly two connections.\n";
            return 0;
        }

        // Choose one direction to start
        int currentDir = startDirs[0];
        std::pair<int, int> currentPos = startPos;
        std::pair<int, int> nextPos = {currentPos.first + move[currentDir].first, currentPos.second + move[currentDir].second};
        path.positions.emplace_back(nextPos);
        visited.insert(nextPos);

        // Previous position
        std::pair<int, int> prevPos = currentPos;
        currentPos = nextPos;

        while (!(currentPos.first == startPos.first && currentPos.second == startPos.second)) {
            // Determine incoming direction
            int dx = currentPos.first - prevPos.first;
            int dy = currentPos.second - prevPos.second;

            int incomingDir;
            if (dx == -1 && dy == 0) incomingDir = SOUTH;
            else if (dx == 1 && dy == 0) incomingDir = NORTH;
            else if (dx == 0 && dy == 1) incomingDir = WEST;
            else if (dx == 0 && dy == -1) incomingDir = EAST;
            else {
                std::cerr << "Error: Invalid movement from (" << prevPos.first << "," << prevPos.second << ") to (" << currentPos.first << "," << currentPos.second << ")\n";
                return 0;
            }

            // Get current tile's outgoing direction
            char currentTile = get(currentPos.first, currentPos.second);
            int outgoingDir = getNextDirection(currentTile, incomingDir);
            if (outgoingDir == -1) {
                std::cerr << "Error: No outgoing direction from (" << currentPos.first << "," << currentPos.second << ")\n";
                return 0;
            }

            // Determine the next position based on outgoing direction
            std::pair<int, int> potentialNextPos = {currentPos.first + move[outgoingDir].first, currentPos.second + move[outgoingDir].second};

            // Check if we've returned to the start
            if (potentialNextPos.first == startPos.first && potentialNextPos.second == startPos.second) {
                path.positions.emplace_back(potentialNextPos);
                break;
            }

            // Check if the next position is already visited (loop integrity)
            if (visited.find(potentialNextPos) != visited.end()) {
                std::cerr << "Error: Encountered a previously visited tile at (" << potentialNextPos.first << "," << potentialNextPos.second << ")\n";
                return 0;
            }

            // Append to path and mark as visited
            path.positions.emplace_back(potentialNextPos);
            visited.insert(potentialNextPos);

            // Update previous and current positions
            prevPos = currentPos;
            currentPos = potentialNextPos;
        }

        return path.positions.size();
    }

    // Compute area using Shoelace formula
    double computeArea(const Path &path) const {
        double area = 0.0;
        int n = path.positions.size();
        for(int i = 0; i < n; ++i){
            int j = (i +1) % n;
            area += (path.positions[i].second * path.positions[j].first) - (path.positions[j].second * path.positions[i].first);
        }
        return std::abs(area) / 2.0;
    }

    // Compute inner tiles using Pick's Theorem: I = A - B/2 +1
    int computeInnerTiles(double area, int boundaryTiles) const {
        // According to Pick's Theorem:
        // I = A - B/2 +1
        double inner = area - (static_cast<double>(boundaryTiles) / 2.0) + 1.0;
        return static_cast<int>(std::round(inner));
    }

    // Navigate the maze and collect the loop path
    int navigateMaze(Path &path) {
        int steps = traverseLoopStepByStep(path);
        return steps;
    }
};

int main() {    
    Maze maze;
    std::string line;
    // Read the maze input from standard input
    while (std::getline(std::cin, line)) {
        maze.insertRow(line);
    }

    Path path;
    // Navigate the maze to mark the loop and collect the path
    int steps = maze.navigateMaze(path);

    if (steps == 0) {
        std::cerr << "Failed to navigate the maze correctly.\n";
        return 1;
    }    

    // Part 2: Compute the number of enclosed tiles using Pick's Theorem
    double area = maze.computeArea(path);
    int boundaryTiles = path.positions.size();
    int innerTiles = maze.computeInnerTiles(area, boundaryTiles);
    std::cout << innerTiles << std::endl;

    return 0;
}
