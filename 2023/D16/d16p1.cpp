#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

// Structure to represent a beam's state
struct Beam {
    int row;
    int col;
    int dir; // 0: right, 1: down, 2: left, 3: up
};

int main() {
    std::vector<std::string> grid;
    std::string line;
    while (std::getline(std::cin, line)) {
        grid.push_back(line);
    }

    int rows = grid.size();
    int cols = grid[0].size();

    // Function to simulate the beam's movement
    auto simulate = [&](Beam start) -> int {
        std::vector<std::vector<std::vector<bool>>> visited(
            rows, std::vector<std::vector<bool>>(cols, std::vector<bool>(4, false))
        );
        std::vector<std::vector<bool>> energized(rows, std::vector<bool>(cols, false));
        std::queue<Beam> q;

        q.push(start);
        if (start.row >=0 && start.row < rows && start.col >= 0 && start.col < cols)
        {
            visited[start.row][start.col][start.dir] = true;
        }

        while (!q.empty()) {
            Beam current = q.front();
            q.pop();

            if (current.row >=0 && current.row < rows && current.col >= 0 && current.col < cols)
            {
                energized[current.row][current.col] = true;
            }

            int next_row = current.row;
            int next_col = current.col;
            int next_dir = current.dir;

            // Move the beam based on direction
            if (current.dir == 0) next_col++;
            else if (current.dir == 1) next_row++;
            else if (current.dir == 2) next_col--;
            else if (current.dir == 3) next_row--;

            // Check if the beam is out of bounds
            if (next_row < 0 || next_row >= rows || next_col < 0 || next_col >= cols) continue;

            // Handle beam interaction with grid elements
            char tile = grid[next_row][next_col];
            if (tile == '.') {
                if (!visited[next_row][next_col][next_dir]) {
                    visited[next_row][next_col][next_dir] = true;
                    q.push({next_row, next_col, next_dir});
                }
            } else if (tile == '/') {
                next_dir = (current.dir == 0) ? 3 : (current.dir == 1) ? 2 : (current.dir == 2) ? 1 : 0;
                if (!visited[next_row][next_col][next_dir]) {
                    visited[next_row][next_col][next_dir] = true;
                    q.push({next_row, next_col, next_dir});
                }
            } else if (tile == '\\') {
                next_dir = (current.dir == 0) ? 1 : (current.dir == 1) ? 0 : (current.dir == 2) ? 3 : 2;
                if (!visited[next_row][next_col][next_dir]) {
                    visited[next_row][next_col][next_dir] = true;
                    q.push({next_row, next_col, next_dir});
                }
            } else if (tile == '-') {
                if (current.dir == 0 || current.dir == 2) {
                    if (!visited[next_row][next_col][next_dir]) {
                        visited[next_row][next_col][next_dir] = true;
                        q.push({next_row, next_col, next_dir});
                    }
                } else {
                    bool vis1 = visited[next_row][next_col][0];
                    bool vis2 = visited[next_row][next_col][2];

                    if (!vis1)
                    {
                        visited[next_row][next_col][0] = true;
                        q.push({next_row, next_col, 0});
                    }
                    if (!vis2)
                    {
                        visited[next_row][next_col][2] = true;
                        q.push({next_row, next_col, 2});
                    }
                }
            } else if (tile == '|') {
                if (current.dir == 1 || current.dir == 3) {
                    if (!visited[next_row][next_col][next_dir]) {
                        visited[next_row][next_col][next_dir] = true;
                        q.push({next_row, next_col, next_dir});
                    }
                } else {
                    bool vis1 = visited[next_row][next_col][1];
                    bool vis2 = visited[next_row][next_col][3];

                    if (!vis1)
                    {
                        visited[next_row][next_col][1] = true;
                        q.push({next_row, next_col, 1});
                    }

                    if (!vis2)
                    {
                        visited[next_row][next_col][3] = true;
                        q.push({next_row, next_col, 3});
                    }
                }
            }
        }

        // Count energized tiles
        int count = 0;
        for(int i = 0; i < rows; ++i) {
            for(int j = 0; j < cols; ++j) {
                if (energized[i][j]) {
                    count++;
                }
            }
        }
        return count;
    };

    // Initial beam starting from top-left and moving right
    int energized_tiles = simulate({0, -1, 0});
    std::cout << energized_tiles << std::endl;

    return 0;
}
