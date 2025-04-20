#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <limits>
#include <cctype>

// Solve Day 24 Part Two: return to start after visiting all points
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

    // Gather all numbered points (including '0') and sort by label
    std::vector<std::pair<char, std::pair<int,int>>> labeled;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (std::isdigit(grid[i][j])) {
                labeled.emplace_back(grid[i][j], std::make_pair(i, j));
            }
        }
    }
    std::sort(labeled.begin(), labeled.end(),
              [](auto &a, auto &b){ return a.first < b.first; });
    int n = static_cast<int>(labeled.size());
    std::vector<std::pair<int,int>> points(n);
    for (int i = 0; i < n; ++i) {
        points[i] = labeled[i].second;
    }

    // Precompute pairwise shortest-path distances with BFS
    const int INF = std::numeric_limits<int>::max();
    std::vector<std::vector<int>> dist(n, std::vector<int>(n, INF));
    std::vector<std::vector<int>> dgrid(h, std::vector<int>(w));
    std::queue<std::pair<int,int>> bfs;
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    for (int src = 0; src < n; ++src) {
        // reset distances on the grid
        for (int i = 0; i < h; ++i) {
            std::fill(dgrid[i].begin(), dgrid[i].end(), -1);
        }
        while (!bfs.empty()) bfs.pop();

        auto [sx, sy] = points[src];
        dgrid[sx][sy] = 0;
        bfs.emplace(sx, sy);

        while (!bfs.empty()) {
            auto [x, y] = bfs.front();
            bfs.pop();
            int cd = dgrid[x][y];
            for (int k = 0; k < 4; ++k) {
                int nx = x + dr[k], ny = y + dc[k];
                if (nx >= 0 && nx < h && ny >= 0 && ny < w
                    && dgrid[nx][ny] < 0
                    && grid[nx][ny] != '#') {
                    dgrid[nx][ny] = cd + 1;
                    bfs.emplace(nx, ny);
                }
            }
        }
        // extract distances to all other points
        for (int dst = 0; dst < n; ++dst) {
            auto [tx, ty] = points[dst];
            if (dgrid[tx][ty] >= 0) {
                dist[src][dst] = dgrid[tx][ty];
            }
        }
    }

    // DP over subsets: dp[mask][u] = min cost to start at 0, visit mask, end at u
    int fullMask = (1 << n);
    std::vector<std::vector<int>> dp(fullMask, std::vector<int>(n, INF));
    dp[1 << 0][0] = 0;

    for (int mask = 0; mask < fullMask; ++mask) {
        if (!(mask & 1)) continue;  // must include starting point
        for (int u = 0; u < n; ++u) {
            if (!(mask & (1 << u))) continue;
            int cur = dp[mask][u];
            if (cur == INF) continue;
            for (int v = 0; v < n; ++v) {
                if (mask & (1 << v)) continue;
                int d = dist[u][v];
                if (d < INF) {
                    int nextMask = mask | (1 << v);
                    dp[nextMask][v] = std::min(dp[nextMask][v], cur + d);
                }
            }
        }
    }

    // Compute answer by returning to 0 after visiting all points
    int answer = INF;
    int finalMask = fullMask - 1;
    for (int u = 0; u < n; ++u) {
        if (dp[finalMask][u] < INF && dist[u][0] < INF) {
            answer = std::min(answer, dp[finalMask][u] + dist[u][0]);
        }
    }

    std::cout << answer << std::endl;
    return 0;
}

