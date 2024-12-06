#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <cmath>
#include <algorithm>

struct pair_hash {
    std::size_t operator()(const std::pair<int,int> &p) const {
        auto h1 = std::hash<int>()(p.first);
        auto h2 = std::hash<int>()(p.second);
        return h1 ^ (h2 + 0x9e3779b97f4a7c16ULL + (h1 << 6) + (h1 >> 2));
    }
};

struct MoveSet {
    int connectA;
    int connectB;
    char mazeChar;
};

class Maze {
public:
    enum DIRECTIONS { NORTH=0, SOUTH=1, EAST=2, WEST=3 };

    void insertRow(const std::string &row) {
        board.emplace_back(row.begin(), row.end());
    }

    int solve() {
        findStart();
        if (startPos.first == -1) return 0;

        // Determine S's correct symbol
        replaceSWithCorrectSymbol();

        // After replacing S, find the loop
        auto loopPath = findLoop();
        if (loopPath.empty()) return 0;

        int R = (int)board.size();
        int C = (int)board[0].size();

        // Build polygon from loop tiles
        // polygon vertices: (x,y) = (col+0.5, row+0.5)
        std::vector<std::pair<double,double>> polygon;
        for (int i = 0; i < (int)loopPath.size()-1; i++) {
            int r = loopPath[i].first;
            int c = loopPath[i].second;
            polygon.push_back({c + 0.5, r + 0.5});
        }

        // Count enclosed '.' tiles
        int enclosedCount = 0;
        for (int i = 0; i < R; i++) {
            for (int j = 0; j < C; j++) {
                if (board[i][j] == '.') {
                    double px = j + 0.5;
                    double py = i + 0.5;
                    if (pointInPolygon(px, py, polygon)) {
                        enclosedCount++;
                    }
                }
            }
        }

        return enclosedCount;
    }

private:
    std::vector<std::vector<char>> board;
    std::pair<int,int> startPos = {-1,-1};

    std::vector<int> inverse = {SOUTH, NORTH, WEST, EAST};
    std::vector<std::pair<int,int>> move = {
        {-1,0}, // NORTH
        {1,0},  // SOUTH
        {0,1},  // EAST
        {0,-1}  // WEST
    };

    std::vector<MoveSet> mazeMoves = {
        {NORTH, SOUTH, '|'},
        {EAST, WEST, '-'},
        {NORTH, EAST, 'L'},
        {NORTH, WEST, 'J'},
        {SOUTH, EAST, 'F'},
        {SOUTH, WEST, '7'}
    };

    void findStart() {
        for (int i = 0; i < (int)board.size(); i++) {
            for (int j = 0; j < (int)board[i].size(); j++) {
                if (board[i][j] == 'S') {
                    startPos = {i,j};
                    return;
                }
            }
        }
    }

    char get(int i, int j) const {
        if (i < 0 || i >= (int)board.size() || j < 0 || j >= (int)board[0].size()) return '\0';
        return board[i][j];
    }

    int getNextDirection(char pipeChar, int incomingDir) {
        for (auto &m : mazeMoves) {
            if (m.mazeChar == pipeChar) {
                if (m.connectA == incomingDir) return m.connectB;
                if (m.connectB == incomingDir) return m.connectA;
            }
        }
        return -1;
    }

    std::vector<int> getPossibleDirectionsForS(int si, int sj) {
        std::vector<int> dirs;
        for (int d = 0; d < 4; d++) {
            int ni = si + move[d].first;
            int nj = sj + move[d].second;
            char c = get(ni,nj);
            if (c == '\0' || c == '.') continue;
            int inc = inverse[d];
            int out = getNextDirection(c, inc);
            if (out != -1) dirs.push_back(d);
        }
        return dirs;
    }

    // Determine S's correct symbol by analyzing its connections
    void replaceSWithCorrectSymbol() {
        if (startPos.first == -1) return;
        int si = startPos.first;
        int sj = startPos.second;

        auto sDirs = getPossibleDirectionsForS(si, sj);
        if (sDirs.size() != 2) {
            // If not exactly two directions, can't form a proper loop
            return;
        }

        // Sort to avoid confusion
        std::sort(sDirs.begin(), sDirs.end());

        // sDirs contains two distinct directions, find matching pipe
        // Directions are NORTH=0,SOUTH=1,EAST=2,WEST=3
        // Match to pipe chars:
        // (NORTH,SOUTH) -> '|'
        // (EAST,WEST) -> '-'
        // (NORTH,EAST) -> 'L'
        // (NORTH,WEST) -> 'J'
        // (SOUTH,EAST) -> 'F'
        // (SOUTH,WEST) -> '7'

        int d1 = sDirs[0], d2 = sDirs[1];
        char sChar = '.';
        if (d1 == NORTH && d2 == SOUTH) sChar = '|';
        else if (d1 == EAST && d2 == WEST) sChar = '-';
        else if (d1 == NORTH && d2 == EAST) sChar = 'L';
        else if (d1 == NORTH && d2 == WEST) sChar = 'J';
        else if (d1 == SOUTH && d2 == EAST) sChar = 'F';
        else if (d1 == SOUTH && d2 == WEST) sChar = '7';

        if (sChar != '.') {
            board[si][sj] = sChar;
        }
    }

    std::vector<std::pair<int,int>> findLoop() {
        if (startPos.first == -1) return {};
        int si = startPos.first;
        int sj = startPos.second;

        // After replacing S, get its direction again
        auto startDirs = getPossibleDirectionsForS(si, sj);
        if (startDirs.empty()) return {};

        int currentDir = startDirs[0];
        std::pair<int,int> pos = startPos;
        std::unordered_map<std::pair<int,int>, bool, pair_hash> visited;
        visited[pos] = true;

        std::vector<std::pair<int,int>> path;
        path.push_back(pos);
        bool started = false;

        while (true) {
            int ni = pos.first + move[currentDir].first;
            int nj = pos.second + move[currentDir].second;
            char nextTile = get(ni,nj);
            if (nextTile == '\0' || nextTile == '.') break;

            std::pair<int,int> nextPos = {ni,nj};
            if (nextTile == 'S') {
                // Should no longer have 'S' at this point since replaced, but just in case:
                break;
            }

            if (nextTile == board[startPos.first][startPos.second] && started) {
                // Returned to start tile
                path.push_back(nextPos);
                break;
            }

            if (visited.find(nextPos)!=visited.end() && (ni!=si || nj!=sj)) break;

            int inc = inverse[currentDir];
            int outDir = getNextDirection(nextTile, inc);
            if (outDir == -1) break;

            visited[nextPos] = true;
            path.push_back(nextPos);
            pos = nextPos;
            currentDir = outDir;
            started = true;
        }

        // Check if loop closed properly
        if (path.empty() || path.front()!=path.back()) return {};
        return path;
    }

    bool pointInPolygon(double x, double y, const std::vector<std::pair<double,double>> &polygon) {
        int count = 0;
        int n = (int)polygon.size();
        for (int i = 0; i < n; i++) {
            int j = (i+1)%n;
            double x1 = polygon[i].first; 
            double y1 = polygon[i].second;
            double x2 = polygon[j].first; 
            double y2 = polygon[j].second;

            if (((y1 <= y && y < y2) || (y2 <= y && y < y1)) &&
                (x < (x2 - x1)*(y - y1)/(y2 - y1) + x1)) {
                count++;
            }
        }
        return (count % 2) == 1;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    Maze maze;
    std::string line;
    while (std::getline(std::cin, line)) {
        maze.insertRow(line);
    }

    int result = maze.solve();
    std::cout << result << "\n";

    return 0;
}
