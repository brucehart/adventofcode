#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

int main() {
    std::vector<std::string> grid;
    std::string line;
    while (std::getline(std::cin, line)) {
        grid.push_back(line);
    }

    int start_row = -1, start_col = -1;
    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j] == 'S') {
                start_row = i;
                start_col = j;
                break;
            }
        }
        if (start_row != -1) break;
    }

    int rows = grid.size();
    int cols = grid[0].size();
    int steps = 64;

    std::queue<std::pair<int, int>> q;
    q.push({start_row, start_col});

    std::vector<std::vector<int>> dist(rows, std::vector<int>(cols, -1));
    dist[start_row][start_col] = 0;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!q.empty()) {
        int r = q.front().first;
        int c = q.front().second;
        q.pop();

        if (dist[r][c] == steps) continue;

        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && 
                grid[nr][nc] != '#' && dist[nr][nc] == -1) {
                dist[nr][nc] = dist[r][c] + 1;
                q.push({nr, nc});
            }
        }
    }

    int count = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (dist[i][j] != -1 && dist[i][j] <= steps && dist[i][j] % 2 == 0) {
                count++;
            }
        }
    }

    std::cout << count << std::endl;

    return 0;
}
