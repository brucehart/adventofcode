#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <limits>

struct Node {
    int d;
    int idx;
};

struct Cmp {
    bool operator()(const Node& a, const Node& b) const {
        return a.d > b.d;
    }
};

int main() {
    std::vector<std::string> grid;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        grid.push_back(line);
    }
    if (grid.empty()) return 0;

    const int h = static_cast<int>(grid.size());
    const int w = static_cast<int>(grid[0].size());
    for (const auto& row : grid) {
        if (static_cast<int>(row.size()) != w) return 0;
    }

    const int n = w * h;
    const int INF = std::numeric_limits<int>::max() / 4;
    std::vector<int> dist(n, INF);

    std::priority_queue<Node, std::vector<Node>, Cmp> pq;
    dist[0] = 0;
    pq.push({0, 0});

    auto relax = [&](int from, int to, int cost) {
        if (dist[from] + cost < dist[to]) {
            dist[to] = dist[from] + cost;
            pq.push({dist[to], to});
        }
    };

    while (!pq.empty()) {
        const Node cur = pq.top();
        pq.pop();
        if (cur.d != dist[cur.idx]) continue;
        if (cur.idx == n - 1) break;

        const int y = cur.idx / w;
        const int x = cur.idx % w;

        if (x > 0) {
            const int to = cur.idx - 1;
            relax(cur.idx, to, grid[y][x - 1] - '0');
        }
        if (x + 1 < w) {
            const int to = cur.idx + 1;
            relax(cur.idx, to, grid[y][x + 1] - '0');
        }
        if (y > 0) {
            const int to = cur.idx - w;
            relax(cur.idx, to, grid[y - 1][x] - '0');
        }
        if (y + 1 < h) {
            const int to = cur.idx + w;
            relax(cur.idx, to, grid[y + 1][x] - '0');
        }
    }

    std::cout << dist[n - 1] << std::endl;
    return 0;
}

