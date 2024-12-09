#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

// Structure to represent a state in the search space
struct State {
    int row;
    int col;
    int dir; // 0: up, 1: right, 2: down, 3: left
    int steps; // Number of steps taken in the current direction
    int heatLoss;

    // Constructor
    State(int r, int c, int d, int s, int h) : row(r), col(c), dir(d), steps(s), heatLoss(h) {}

    // Comparison operator for priority queue (min-heap based on heat loss)
    bool operator>(const State& other) const {
        return heatLoss > other.heatLoss;
    }
};

int main() {
    std::vector<std::vector<int>> grid;
    std::string line;

    // Read input grid
    while (std::getline(std::cin, line)) {
        std::vector<int> row;
        for (char c : line) {
            row.push_back(c - '0');
        }
        grid.push_back(row);
    }

    int rows = grid.size();
    int cols = grid[0].size();

    // Directions (up, right, down, left)
    int dr[] = {-1, 0, 1, 0};
    int dc[] = {0, 1, 0, -1};

    // Priority queue for A* search (min-heap)
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;

    // Visited states: visited[row][col][dir][steps]
    std::vector<std::vector<std::vector<std::vector<bool>>>> visited(
        rows, std::vector<std::vector<std::vector<bool>>>(
            cols, std::vector<std::vector<bool>>(
                4, std::vector<bool>(4, false)
            )
        )
    );

    // Start states (can start moving right or down)
    pq.push(State(0, 0, 1, 0, 0)); 
    pq.push(State(0, 0, 2, 0, 0));

    int minHeatLoss = -1;

    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        // Check if reached the destination
        if (current.row == rows - 1 && current.col == cols - 1) {
            minHeatLoss = current.heatLoss;
            break;
        }

        // Skip if already visited with the same or less heat loss
        if (visited[current.row][current.col][current.dir][current.steps]) {
            continue;
        }
        visited[current.row][current.col][current.dir][current.steps] = true;

        // Explore possible moves
        for (int newDir = 0; newDir < 4; ++newDir) {
            // Can't reverse direction
            if ((current.dir + 2) % 4 == newDir) {
                continue;
            }

            int newSteps = (newDir == current.dir) ? current.steps + 1 : 1;

            // Can't move more than 3 steps in the same direction
            if (newSteps > 3) {
                continue;
            }

            int newRow = current.row + dr[newDir];
            int newCol = current.col + dc[newDir];

            // Check if the new position is within the grid
            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
                int newHeatLoss = current.heatLoss + grid[newRow][newCol];
                pq.push(State(newRow, newCol, newDir, newSteps, newHeatLoss));
            }
        }
    }

    std::cout << minHeatLoss << std::endl;

    return 0;
}
