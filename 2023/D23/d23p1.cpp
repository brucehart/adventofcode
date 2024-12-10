#include <iostream>
#include <vector>
#include <algorithm>

// Function to find the longest hike
int findLongestHike(std::vector<std::string>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();

    // Find start and end points
    int start_col = -1;
    int end_col = -1;
    for (int j = 0; j < cols; ++j) {
        if (grid[0][j] == '.') {
            start_col = j;
        }
        if (grid[rows - 1][j] == '.') {
            end_col = j;
        }
    }

    int max_steps = 0;

    // DFS to explore all possible paths
    std::function<void(int, int, int, std::vector<std::vector<bool>>)> dfs = 
        [&](int r, int c, int steps, std::vector<std::vector<bool>> visited) {
        if (r < 0 || r >= rows || c < 0 || c >= cols || grid[r][c] == '#' || visited[r][c]) {
            return;
        }

        visited[r][c] = true;

        if (r == rows - 1 && c == end_col) {
            max_steps = std::max(max_steps, steps);
            return;
        }

        // Explore neighbors based on slopes
        if (grid[r][c] == '.') {
            dfs(r + 1, c, steps + 1, visited);
            dfs(r - 1, c, steps + 1, visited);
            dfs(r, c + 1, steps + 1, visited);
            dfs(r, c - 1, steps + 1, visited);
        } else if (grid[r][c] == 'v') {
            dfs(r + 1, c, steps + 1, visited);
        } else if (grid[r][c] == '^') {
            dfs(r - 1, c, steps + 1, visited);
        } else if (grid[r][c] == '>') {
            dfs(r, c + 1, steps + 1, visited);
        } else if (grid[r][c] == '<') {
            dfs(r, c - 1, steps + 1, visited);
        }
    };

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    dfs(0, start_col, 0, visited);

    return max_steps;
}

int main() {
    std::vector<std::string> grid;
    std::string line;
    while (std::getline(std::cin, line)) {
        grid.push_back(line);
    }

    int longest_hike = findLongestHike(grid);
    std::cout << longest_hike << std::endl;

    return 0;
}
