#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <limits>
#include <cctype>

// Solve Day 24: Air Duct Spelunking
int main() {
    std::vector<std::string> grid;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (!line.empty()) {
            grid.push_back(line);
        }
    }
    int h = static_cast<int>(grid.size());
    if (h == 0) {
        return 0;
    }
    int w = static_cast<int>(grid[0].size());

    // Collect labeled points (digits) with their positions
    std::vector<std::pair<char, std::pair<int,int>>> labeled;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (std::isdigit(grid[i][j])) {
                labeled.emplace_back(grid[i][j], std::make_pair(i, j));
            }
        }
    }
    // Sort by label so that '0' is index 0, '1' is index 1, etc.
    std::sort(labeled.begin(), labeled.end(),
              [](auto &a, auto &b){ return a.first < b.first; });
    int n = static_cast<int>(labeled.size());
    std::vector<std::pair<int,int>> points(n);
    for (int i = 0; i < n; ++i) {
        points[i] = labeled[i].second;
    }

    // Precompute pairwise shortest distances via BFS on grid
    const int INF = std::numeric_limits<int>::max();
    std::vector<std::vector<int>> dist(n, std::vector<int>(n, INF));
    std::vector<std::vector<int>> dgrid(h, std::vector<int>(w, -1));
    std::queue<std::pair<int,int>> bfs;

    for (int src = 0; src < n; ++src) {
        // Reset distance grid
        for (int i = 0; i < h; ++i) {
            std::fill(dgrid[i].begin(), dgrid[i].end(), -1);
        }
        while (!bfs.empty()) {
            bfs.pop();
        }

        auto [sx, sy] = points[src];
        dgrid[sx][sy] = 0;
        bfs.emplace(sx, sy);

        // Standard 4-directional BFS
        const int dr[4] = {-1, 1, 0, 0};
        const int dc[4] = {0, 0, -1, 1};
        while (!bfs.empty()) {
            auto [x, y] = bfs.front();
            bfs.pop();
            int cd = dgrid[x][y];
            for (int k = 0; k < 4; ++k) {
                int nx = x + dr[k];
                int ny = y + dc[k];
                if (nx >= 0 && nx < h && ny >= 0 && ny < w
                    && dgrid[nx][ny] < 0
                    && grid[nx][ny] != '#') {
                    dgrid[nx][ny] = cd + 1;
                    bfs.emplace(nx, ny);
                }
            }
        }

        // Record distances to other labeled points
        for (int dst = 0; dst < n; ++dst) {
            auto [tx, ty] = points[dst];
            if (dgrid[tx][ty] >= 0) {
                dist[src][dst] = dgrid[tx][ty];
            }
        }
    }

    // DP over subsets (bitmask) to solve TSP-like visiting all points
    int fullMask = (1 << n);
    std::vector<std::vector<int>> dp(fullMask, std::vector<int>(n, INF));
    dp[1 << 0][0] = 0;  // start at point '0'

    for (int mask = 0; mask < fullMask; ++mask) {
        if (!(mask & 1)) {
            continue;  // must always include starting point
        }
        for (int u = 0; u < n; ++u) {
            if (!(mask & (1 << u))) {
                continue;
            }
            int cur = dp[mask][u];
            if (cur == INF) {
                continue;
            }
            // Try to go to any unvisited point
            for (int v = 0; v < n; ++v) {
                if (mask & (1 << v)) {
                    continue;
                }
                int d = dist[u][v];
                if (d < INF) {
                    int nextMask = mask | (1 << v);
                    dp[nextMask][v] = std::min(dp[nextMask][v], cur + d);
                }
            }
        }
    }

    // Find minimum cost to visit all points (ending anywhere)
    int answer = INF;
    int finalMask = fullMask - 1;
    for (int u = 0; u < n; ++u) {
        answer = std::min(answer, dp[finalMask][u]);
    }

    std::cout << answer << std::endl;
    return 0;
}

