#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

// Function to find the longest hike (Part 2 with optimizations)
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

    // 1. Build a graph of intersections
    std::map<std::pair<int, int>, std::map<std::pair<int, int>, int>> graph;
    std::vector<std::pair<int, int>> intersections = {{0, start_col}, {rows - 1, end_col}};

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == '#') continue;
            int neighbors = 0;
            if (r > 0 && grid[r - 1][c] != '#') neighbors++;
            if (r < rows - 1 && grid[r + 1][c] != '#') neighbors++;
            if (c > 0 && grid[r][c - 1] != '#') neighbors++;
            if (c < cols - 1 && grid[r][c + 1] != '#') neighbors++;
            if (neighbors > 2) {
                intersections.push_back({r, c});
            }
        }
    }
    
    // For each intersection, find distances to other reachable intersections
    for (auto& start_node : intersections) {
        std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
        std::vector<std::tuple<int, int, int>> q = {{start_node.first, start_node.second, 0}};
        visited[start_node.first][start_node.second] = true;

        int head = 0;
        while (head < q.size()) {
            auto [r, c, dist] = q[head++];
            if (dist != 0 && std::find(intersections.begin(), intersections.end(), std::make_pair(r,c)) != intersections.end()) {
                graph[start_node][{r, c}] = dist;
                continue;
            }

            int dr[] = {-1, 1, 0, 0};
            int dc[] = {0, 0, -1, 1};
            for (int i = 0; i < 4; ++i) {
                int nr = r + dr[i];
                int nc = c + dc[i];
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc] != '#' && !visited[nr][nc]) {
                    visited[nr][nc] = true;
                    q.push_back({nr, nc, dist + 1});
                }
            }
        }
    }

    // 2. DFS on the compressed graph
    int max_steps = 0;
    std::function<void(std::pair<int,int>, int, std::vector<std::pair<int,int>>)> dfs = 
        [&](std::pair<int, int> u, int steps, std::vector<std::pair<int,int>> visited_nodes) {

        if (u.first == rows - 1 && u.second == end_col) {
            max_steps = std::max(max_steps, steps);
            return;
        }

        visited_nodes.push_back(u);
        
        for (auto& [v, weight] : graph[u]) {
            if (std::find(visited_nodes.begin(), visited_nodes.end(), v) == visited_nodes.end()){
                dfs(v, steps + weight, visited_nodes);
            }
        }
    };

    dfs({0, start_col}, 0, {});

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
