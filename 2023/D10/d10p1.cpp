#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

struct MoveSet {
    int connectA;
    int connectB;
    char mazeChar;
};

struct pair_hash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        // A simple hash combination for pairs
        auto h1 = std::hash<int>()(p.first);
        auto h2 = std::hash<int>()(p.second);
        return h1 ^ (h2 + 0x9e3779b97f4a7c16ULL + (h1 << 6) + (h1 >> 2));
    }
};

struct Maze {
    enum DIRECTIONS { NORTH = 0, SOUTH = 1, EAST = 2, WEST = 3 };
    std::vector<int> inverse = { SOUTH, NORTH, WEST, EAST };
    std::vector<std::pair<int, int>> move = {
        {-1, 0},  // NORTH
        {1, 0},   // SOUTH
        {0, 1},   // EAST
        {0, -1}   // WEST
    };

    std::vector<MoveSet> mazeMoves = {
        {NORTH, SOUTH, '|'},
        {EAST, WEST, '-'},
        {NORTH, EAST, 'L'},
        {NORTH, WEST, 'J'},
        {SOUTH, EAST, 'F'},
        {SOUTH, WEST, '7'}
    };

    std::vector<std::vector<char>> board;
    std::unordered_map<std::pair<int,int>, bool, pair_hash> visited;

    std::pair<int, int> startPos;
    int rows() const {
        return (int)board.size();
    }
    int cols() const {
        return (rows() > 0) ? (int)board[0].size() : 0;
    }

    char get(int i, int j) const {
        if (i < 0 || i >= rows() || j < 0 || j >= cols()) {
            return '\0';
        }
        return board[i][j];
    }

    void insertRow(const std::string& rowData) {
        std::vector<char> row(rowData.begin(), rowData.end());
        board.push_back(row);
    }

    void findStart() {
        for (int i = 0; i < rows(); ++i) {
            for (int j = 0; j < cols(); ++j) {
                if (get(i,j) == 'S') {
                    startPos = {i,j};
                    return;
                }
            }
        }
        startPos = {-1,-1};
    }

    // Given a pipe character and an incoming direction, determine the outgoing direction.
    // For example, if we have a '|' pipe and we came from NORTH, we must go SOUTH.
    // If the pipe doesn't match any known pattern, return -1.
    int getNextDirection(char pipeChar, int incomingDir) {
        // incomingDir is the direction from which we entered the pipe.
        for (auto &m : mazeMoves) {
            if (m.mazeChar == pipeChar) {
                // The pipe connects connectA <-> connectB.
                // If we came from connectA, we go out connectB.
                // If we came from connectB, we go out connectA.
                if (m.connectA == incomingDir) {
                    return m.connectB;
                } else if (m.connectB == incomingDir) {
                    return m.connectA;
                }
            }
        }
        return -1; // no valid connection found
    }

    // For the starting tile 'S', we need to figure out what kind of pipe it is.
    // We do this by checking what directions are connected. 
    // S should function just like one of the known pipe characters. To find its shape:
    // - Check each direction to see if there's a pipe that can connect back.
    // We'll return all directions that can form a valid connection with S.
    std::vector<int> getPossibleDirectionsForS(int si, int sj) {
        std::vector<int> directions;
        for (int d = 0; d < 4; ++d) {
            int ni = si + move[d].first;
            int nj = sj + move[d].second;
            char c = get(ni,nj);
            if (c == '\0' || c == '.' ) continue;
            // Check if that pipe can connect to us if we treat 'S' as a pipe tile.
            // We pretend S is unknown but must be compatible. For that tile to connect to S,
            // it must have a connection coming from the opposite direction of d (inverse[d]).
            int incDir = inverse[d]; // direction from that tile back to S
            int outDir = getNextDirection(c, incDir);
            if (outDir != -1) {
                // This means if we stood on that tile, and came from incDir,
                // we could leave in outDir. Which also means from S's perspective,
                // if we go in direction d, we reach a pipe that would have connected back.
                directions.push_back(d);
            }
        }
        return directions;
    }

    int navigateMaze() {
        findStart();
        if (startPos.first == -1) {
            // No start found
            return 0;
        }

        int si = startPos.first;
        int sj = startPos.second;

        // Determine possible initial directions from S.
        std::vector<int> startDirs = getPossibleDirectionsForS(si, sj);
        // According to the puzzle, S is part of a single loop and should have exactly two connections.
        // We'll start by taking one direction and begin the traversal.
        if (startDirs.empty()) {
            // No valid start direction means no loop
            return 0;
        }

        // We'll choose the first direction from S as our starting direction.
        int currentDir = startDirs[0];
        std::pair<int,int> pos = startPos;
        visited[pos] = true;
        bool started = false;
        int steps = 0;

        while (true) {
            // Move to the next tile in currentDir
            int ni = pos.first + move[currentDir].first;
            int nj = pos.second + move[currentDir].second;
            char nextTile = get(ni, nj);

            if (nextTile == '\0' || nextTile == '.') {
                // No valid tile to move to; this should not happen in a proper loop
                break;
            }

            std::pair<int,int> nextPos = {ni, nj};

            // If we've returned to S after starting, loop is closed
            if (nextTile == 'S' && started) {
                steps++;
                return steps; // Completed the loop
            }

            // If we've seen this tile before and it's not S, 
            // this means we are looping incorrectly. According to the puzzle, 
            // the loop should return to S, so this would indicate an error.
            if (visited.find(nextPos) != visited.end() && nextTile != 'S') {
                // We've hit a visited tile that isn't S again - abnormal situation
                // Break out to avoid infinite loop
                break;
            }

            // Determine next direction based on the pipe shape.
            // The incoming direction to the new tile is the inverse of currentDir.
            int incomingDir = inverse[currentDir];

            // 'S' should behave like a pipe. If S is encountered before started=true,
            // we must still treat it like a pipe with the known connections from startDirs.
            // If nextTile == 'S' and not started, we can still figure out direction from startDirs.
            int outDir;
            if (nextTile == 'S') {
                // If we're on S again but not started, it means we just moved onto S from start.
                // This can happen if the loop is very small. Treat S as we did at start.
                // Find possible directions again (should be the same two).
                // We must find the direction that is not 'incomingDir' since we must turn around.
                std::vector<int> sDirs = getPossibleDirectionsForS(ni, nj);
                // Among sDirs, find one that isn't incomingDir.
                // Actually, if we just arrived at S, incomingDir is where we came from,
                // we must choose the other direction that forms the loop.
                // However, if we got back to S immediately, that might mean a 2-tile loop.
                // In that case, the other direction from S is currentDir again reversed.
                if (sDirs.size() == 2) {
                    // One of these must be the outgoing direction.
                    // If incomingDir is in sDirs, we choose the other one.
                    if (sDirs[0] == incomingDir) outDir = sDirs[1];
                    else if (sDirs[1] == incomingDir) outDir = sDirs[0];
                    else {
                        // If incomingDir is not in sDirs, we have a configuration error.
                        // Just pick one direction. This scenario should not happen in a valid puzzle.
                        outDir = sDirs[0];
                    }
                } else if (sDirs.size() == 1) {
                    // A degenerate loop with only one connection?
                    // This shouldn't happen in a proper loop puzzle, but handle gracefully.
                    outDir = sDirs[0];
                } else {
                    // No valid direction - break
                    break;
                }
            } else {
                // Normal pipe character
                outDir = getNextDirection(nextTile, incomingDir);
            }

            if (outDir == -1) {
                // No valid direction means we can't continue
                break;
            }

            // Mark visited and update position/direction
            visited[nextPos] = true;
            pos = nextPos;
            currentDir = outDir;
            steps++;
            started = true; // now we have definitely started traveling the loop
        }

        // If we exit the loop without returning to S properly, no proper loop found.
        return 0;
    }
};

int main() {    
    Maze maze;
    std::string line;
    while (std::getline(std::cin, line)) {
        maze.insertRow(line);
    }

    int steps = maze.navigateMaze();
    std::cout << steps/2 << "\n";

    return 0;
}
