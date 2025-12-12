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

static int risk_at(const std::vector<std::string>& base, int x, int y) {
    const int h = static_cast<int>(base.size());
    const int w = static_cast<int>(base[0].size());
    const int add = (x / w) + (y / h);
    const int v0 = base[y % h][x % w] - '0';
    const int v = v0 + add;
    return ((v - 1) % 9) + 1;
}

int main() {
    std::vector<std::string> base;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        base.push_back(line);
    }
    if (base.empty()) return 0;

    const int h0 = static_cast<int>(base.size());
    const int w0 = static_cast<int>(base[0].size());
    for (const auto& row : base) {
        if (static_cast<int>(row.size()) != w0) return 0;
    }

    const int h = h0 * 5;
    const int w = w0 * 5;
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
            relax(cur.idx, to, risk_at(base, x - 1, y));
        }
        if (x + 1 < w) {
            const int to = cur.idx + 1;
            relax(cur.idx, to, risk_at(base, x + 1, y));
        }
        if (y > 0) {
            const int to = cur.idx - w;
            relax(cur.idx, to, risk_at(base, x, y - 1));
        }
        if (y + 1 < h) {
            const int to = cur.idx + w;
            relax(cur.idx, to, risk_at(base, x, y + 1));
        }
    }

    std::cout << dist[n - 1] << std::endl;
    return 0;
}

