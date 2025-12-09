// Advent of Code 2025 - Day 8 Part 1
// Connect the 1000 closest pairs of points and compute the product of the three largest component sizes.

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

    int size(int x) {
        return sz[find(x)];
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
    if (n == 0) return 0;

    const size_t K = 1000;
    const size_t max_pairs = static_cast<size_t>(n) * static_cast<size_t>(n - 1) / 2;
    const size_t target = min(K, max_pairs);

    auto dist2 = [&](int a, int b) -> long long {
        long long dx = pts[a][0] - pts[b][0];
        long long dy = pts[a][1] - pts[b][1];
        long long dz = pts[a][2] - pts[b][2];
        return dx * dx + dy * dy + dz * dz;
    };

    auto better = [](const PairInfo& a, const PairInfo& b) {
        if (a.dist != b.dist) return a.dist < b.dist;
        if (a.i != b.i) return a.i < b.i;
        return a.j < b.j;
    };

    struct HeapCmp {
        bool operator()(const PairInfo& a, const PairInfo& b) const {
            if (a.dist != b.dist) return a.dist < b.dist;
            if (a.i != b.i) return a.i < b.i;
            return a.j < b.j;
        }
    };

    priority_queue<PairInfo, vector<PairInfo>, HeapCmp> pq;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            PairInfo p{dist2(i, j), i, j};
            if (pq.size() < target) {
                pq.push(p);
            } else if (better(p, pq.top())) {
                pq.pop();
                pq.push(p);
            }
        }
    }

    vector<PairInfo> edges;
    edges.reserve(target);
    while (!pq.empty()) {
        edges.push_back(pq.top());
        pq.pop();
    }
    sort(edges.begin(), edges.end(), better);

    DSU dsu(n);
    for (const auto& e : edges) {
        dsu.unite(e.i, e.j);
    }

    vector<int> comp_sizes;
    comp_sizes.reserve(n);
    for (int i = 0; i < n; ++i) {
        if (dsu.find(i) == i) comp_sizes.push_back(dsu.size(i));
    }
    sort(comp_sizes.begin(), comp_sizes.end(), greater<int>());

    long long answer = 1;
    int take = min(3, static_cast<int>(comp_sizes.size()));
    for (int i = 0; i < take; ++i) {
        answer *= comp_sizes[i];
    }

    cout << answer << "\n";
    return 0;
}
