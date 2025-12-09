// Advent of Code 2025 - Day 8 Part 2
// Keep connecting the closest pairs until all junction boxes are in one circuit.
// Output the product of the X coordinates of the final pair connected.

#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> parent;
    vector<int> sz;

    explicit DSU(int n) : parent(n), sz(n, 1) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        parent[b] = a;
        sz[a] += sz[b];
        return true;
    }
};

struct PairInfo {
    long long dist;
    int i;
    int j;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<array<long long, 3>> pts;
    string line;
    while (getline(cin, line)) {
        if (line.empty()) continue;
        replace(line.begin(), line.end(), ',', ' ');
        stringstream ss(line);
        long long x, y, z;
        if (!(ss >> x >> y >> z)) continue;
        pts.push_back({x, y, z});
    }

    int n = static_cast<int>(pts.size());
    if (n <= 1) {
        cout << 0 << "\n";
        return 0;
    }

    auto dist2 = [&](int a, int b) -> long long {
        long long dx = pts[a][0] - pts[b][0];
        long long dy = pts[a][1] - pts[b][1];
        long long dz = pts[a][2] - pts[b][2];
        return dx * dx + dy * dy + dz * dz;
    };

    vector<PairInfo> edges;
    edges.reserve(static_cast<size_t>(n) * static_cast<size_t>(n - 1) / 2);
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
    sort(edges.begin(), edges.end(), cmp);

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

    cout << answer << "\n";
    return 0;
}
