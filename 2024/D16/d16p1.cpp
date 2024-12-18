#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <climits>

// Structure to represent a state in the maze
struct State {
    int row;
    int col;
    int dir; // 0: East, 1: South, 2: West, 3: North
    int cost;

    bool operator>(const State& other) const {
        return cost > other.cost; // For min-heap priority queue
    }
};

int main() {
    std::vector<std::string> maze;
    std::string line;
    int startRow, startCol, endRow, endCol;

    // Read the maze input
    int row = 0;
    while (std::getline(std::cin, line)) {
        for(int col = 0; col < line.size(); ++col) {
            if(line[col] == 'S') {
                startRow = row;
                startCol = col;
            } else if(line[col] == 'E') {
                endRow = row;
                endCol = col;
            }
        }
        maze.push_back(line);
        ++row;
    }
    
    int numRows = maze.size();
    int numCols = maze[0].size();

    // Directions (East, South, West, North)
    int dr[] = {0, 1, 0, -1};
    int dc[] = {1, 0, -1, 0};

    // Initialize priority queue and visited state tracker
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    std::vector<std::vector<std::vector<int>>> visited(numRows, std::vector<std::vector<int>>(numCols, std::vector<int>(4, INT_MAX)));
    
    // Start state with initial cost 0 facing East
    pq.push({startRow, startCol, 0, 0});
    visited[startRow][startCol][0] = 0;
    
    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();
        
        int r = current.row;
        int c = current.col;
        int dir = current.dir;
        int cost = current.cost;
        
        // Goal check
        if (r == endRow && c == endCol) {
            std::cout << cost << std::endl;
            return 0;
        }
        
        // Check if this path is already better
        if (cost > visited[r][c][dir]) {
            continue;
        }

        // Move forward
        int nr = r + dr[dir];
        int nc = c + dc[dir];
        if (nr >= 0 && nr < numRows && nc >= 0 && nc < numCols && maze[nr][nc] != '#') {
            if(cost + 1 < visited[nr][nc][dir]){
                visited[nr][nc][dir] = cost + 1;
                pq.push({nr, nc, dir, cost + 1});
            }
        }

        // Rotate clockwise
        int newDirClockwise = (dir + 1) % 4;
         if(cost + 1000 < visited[r][c][newDirClockwise]){
            visited[r][c][newDirClockwise] = cost + 1000;
            pq.push({r, c, newDirClockwise, cost + 1000});
        }

        // Rotate counterclockwise
        int newDirCounterClockwise = (dir + 3) % 4;
         if(cost + 1000 < visited[r][c][newDirCounterClockwise]){
            visited[r][c][newDirCounterClockwise] = cost + 1000;
            pq.push({r, c, newDirCounterClockwise, cost + 1000});
        }
    }
    
    return 0; 
}
