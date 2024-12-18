#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <climits>
#include <string>

// Structure to represent a state in the maze
struct State {
    int row;
    int col;
    int dir; // 0: East, 1: South, 2: West, 3: North (Forward Traversal)
            // 0: West, 1: North, 2: East, 3: South (Reverse Traversal)
    int cost;

    bool operator>(const State& other) const {
        return cost > other.cost; // For min-heap priority queue
    }
};

// Directions for Forward Traversal (from Start S)
const int dr_forward[] = {0, 1, 0, -1}; // East, South, West, North
const int dc_forward[] = {1, 0, -1, 0};

// Directions for Reverse Traversal (from End E)
const int dr_reverse[] = {0, -1, 0, 1}; // West, North, East, South
const int dc_reverse[] = {-1, 0, 1, 0};

// Function to perform Dijkstra's algorithm
std::vector<std::vector<std::vector<int>>> dijkstra(int startRow, int startCol, const std::vector<std::string>& maze, const int dr[], const int dc[], bool isForward = true) {
    int numRows = maze.size();
    int numCols = maze[0].size();

    // Initialize priority queue and visited state tracker
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    std::vector<std::vector<std::vector<int>>> cost(numRows, std::vector<std::vector<int>>(numCols, std::vector<int>(4, INT32_MAX)));

    if(isForward){
        // Start state with initial direction East (0) and cost 0
        pq.push({startRow, startCol, 0, 0});
        cost[startRow][startCol][0] = 0;
    }
    else{
        // When running from End, initialize all directions as possible incoming directions
        for(int initialDir = 0; initialDir < 4; ++initialDir){
            pq.push({startRow, startCol, initialDir, 0});
            cost[startRow][startCol][initialDir] = 0;
        }
    }

    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        int r = current.row;
        int c = current.col;
        int dir = current.dir;
        int currentCost = current.cost;

        // If this state has already been processed with a lower cost, skip it
        if (currentCost > cost[r][c][dir]) {
            continue;
        }

        // Explore possible actions from the current state

        // 1. Move forward
        int nr = r + dr[dir];
        int nc = c + dc[dir];
        if (nr >= 0 && nr < numRows && nc >= 0 && nc < numCols && maze[nr][nc] != '#') {
            if(currentCost + 1 < cost[nr][nc][dir]){
                cost[nr][nc][dir] = currentCost + 1;
                pq.push({nr, nc, dir, currentCost + 1});
            }
        }

        // 2. Rotate clockwise
        int newDirClockwise = (dir + 1) % 4;
        if(currentCost + 1000 < cost[r][c][newDirClockwise]){
            cost[r][c][newDirClockwise] = currentCost + 1000;
            pq.push({r, c, newDirClockwise, currentCost + 1000});
        }

        // 3. Rotate counterclockwise
        int newDirCounterClockwise = (dir + 3) % 4;
        if(currentCost + 1000 < cost[r][c][newDirCounterClockwise]){
            cost[r][c][newDirCounterClockwise] = currentCost + 1000;
            pq.push({r, c, newDirCounterClockwise, currentCost + 1000});
        }
    }

    return cost;
}

// Function to compute rotation cost between two directions
int rotation_cost(int d_in, int d_out){
    if(d_in == d_out) return 0;
    else if(d_out == (d_in + 1) % 4 || d_out == (d_in + 3) %4) return 1000;
    else return 2000;
}

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

    // First Dijkstra's run from Start to all tiles using Forward Directions
    std::vector<std::vector<std::vector<int>>> cost_from_S = dijkstra(startRow, startCol, maze, dr_forward, dc_forward, true);

    // Find the minimal cost to reach End from Start
    int minimal_cost = INT32_MAX;
    for(int dir = 0; dir < 4; ++dir){
        if(cost_from_S[endRow][endCol][dir] < minimal_cost){
            minimal_cost = cost_from_S[endRow][endCol][dir];
        }
    }

    // Second Dijkstra's run from End to all tiles using Reverse Directions
    std::vector<std::vector<std::vector<int>>> cost_from_E = dijkstra(endRow, endCol, maze, dr_reverse, dc_reverse, false);

    // Now, iterate over all tiles and directions to identify tiles on any minimal path
    std::vector<std::vector<bool>> on_minimal_path(numRows, std::vector<bool>(numCols, false));

    for(int r = 0; r < numRows; ++r){
        for(int c = 0; c < numCols; ++c){
            if(maze[r][c] == '#') continue; // Skip walls
            for(int d_in = 0; d_in < 4; ++d_in){
                for(int d_out = 0; d_out < 4; ++d_out){
                    int total_cost;
                    if(maze[r][c] == 'S' || maze[r][c] == 'E'){
                        // No rotation cost at Start and End tiles
                        total_cost = cost_from_S[r][c][d_in] + cost_from_E[r][c][d_out];
                    }
                    else{
                        // Apply rotation cost for other tiles
                        int rot_cost = rotation_cost(d_in, d_out);
                        total_cost = cost_from_S[r][c][d_in] + rot_cost + cost_from_E[r][c][d_out];
                    }

                    if(total_cost == minimal_cost){
                        on_minimal_path[r][c] = true;
                        break; // No need to check other directions for this tile
                    }
                }
                if(on_minimal_path[r][c]){
                    break; // Move to the next tile
                }
            }
        }
    }

    // Count the number of tiles that are part of at least one minimal path
    int count = 0;
    for(int r = 0; r < numRows; ++r){
        for(int c = 0; c < numCols; ++c){
            if(on_minimal_path[r][c]){
                ++count;
            }
        }
    }

    std::cout << count << std::endl;

    return 0;
}
