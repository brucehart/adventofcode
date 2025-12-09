// Advent of Code 2025 - Day 8 Part 2
// Keep connecting the closest pairs until all junction boxes are in one circuit.
// Output the product of the X coordinates of the final pair connected.

#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

struct DSU {
    std::vector<int> parent;
    std::vector<int> sz;

    explicit DSU(int n) : parent(n), sz(n, 1) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) std::swap(a, b);
        parent[b] = a;
        sz[a] += sz[b];
        return true;
    }
};

struct PairInfo { long long dist; int i; int j; };

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<std::array<long long, 3>> pts;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::replace(line.begin(), line.end(), ',', ' ');
        std::stringstream ss(line);
        long long x, y, z;
        if (!(ss >> x >> y >> z)) continue;
        pts.push_back({x, y, z});
    }

    int n = static_cast<int>(pts.size());
    if (n <= 1) {
        std::cout << 0 << '\n';
        return 0;
    }

    auto dist2 = [&](int a, int b) -> long long {
        long long dx = pts[a][0] - pts[b][0];
        long long dy = pts[a][1] - pts[b][1];
        long long dz = pts[a][2] - pts[b][2];
        return dx * dx + dy * dy + dz * dz;
    };

    std::vector<PairInfo> edges;
    edges.reserve(static_cast<std::size_t>(n) * static_cast<std::size_t>(n - 1) / 2);
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            edges.push_back({dist2(i, j), i, j});
        }
    }

    auto cmp = [](const PairInfo& a, const PairInfo& b) {
        if (a.dist != b.dist) return a.dist < b.dist;
        if (a.i != b.i) return a.i < b.i;
        return a.j < b.j;
    };
    std::sort(edges.begin(), edges.end(), cmp);

    DSU dsu(n);
    int comps = n;
    long long answer = 0;
    for (const auto& e : edges) {
        if (dsu.unite(e.i, e.j)) {
            --comps;
            if (comps == 1) {
                answer = pts[e.i][0] * pts[e.j][0];
                break;
            }
        }
    }

    std::cout << answer << '\n';
    return 0;
}
