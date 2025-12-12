#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cstdint>
#include <algorithm>

static int type_from_char(char c) {
    if (c == 'A') return 1;
    if (c == 'B') return 2;
    if (c == 'C') return 3;
    if (c == 'D') return 4;
    return 0;
}

static long long energy(int t) {
    if (t == 1) return 1;
    if (t == 2) return 10;
    if (t == 3) return 100;
    return 1000;
}

static int get_cell(std::uint64_t s, int idx) {
    return static_cast<int>((s >> (idx * 3)) & 7ULL);
}

static std::uint64_t set_cell(std::uint64_t s, int idx, int v) {
    const std::uint64_t mask = 7ULL << (idx * 3);
    s &= ~mask;
    s |= (static_cast<std::uint64_t>(v) << (idx * 3));
    return s;
}

static bool is_goal(std::uint64_t s) {
    for (int i = 0; i < 11; i++) {
        if (get_cell(s, i) != 0) return false;
    }
    for (int r = 0; r < 4; r++) {
        for (int d = 0; d < 2; d++) {
            const int idx = 11 + r * 2 + d;
            if (get_cell(s, idx) != r + 1) return false;
        }
    }
    return true;
}

static bool hallway_clear(std::uint64_t s, int a, int b) {
    if (a > b) std::swap(a, b);
    for (int x = a + 1; x <= b - 1; x++) {
        if (get_cell(s, x) != 0) return false;
    }
    return true;
}

struct Node {
    long long dist;
    std::uint64_t state;
};

struct Cmp {
    bool operator()(const Node& a, const Node& b) const {
        return a.dist > b.dist;
    }
};

int main() {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        lines.push_back(line);
    }
    if (lines.size() < 5) return 0;

    // Parse room contents from lines[2] and lines[3].
    const int top[4] = {
        type_from_char(lines[2][3]),
        type_from_char(lines[2][5]),
        type_from_char(lines[2][7]),
        type_from_char(lines[2][9]),
    };
    const int bot[4] = {
        type_from_char(lines[3][3]),
        type_from_char(lines[3][5]),
        type_from_char(lines[3][7]),
        type_from_char(lines[3][9]),
    };

    std::uint64_t start = 0;
    for (int r = 0; r < 4; r++) {
        start = set_cell(start, 11 + r * 2 + 0, top[r]);
        start = set_cell(start, 11 + r * 2 + 1, bot[r]);
    }

    static const int doors[4] = {2, 4, 6, 8};
    static const int hall_spots[7] = {0, 1, 3, 5, 7, 9, 10};

    std::priority_queue<Node, std::vector<Node>, Cmp> pq;
    std::unordered_map<std::uint64_t, long long> dist;
    dist.reserve(200000);

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        const Node cur = pq.top();
        pq.pop();
        auto it = dist.find(cur.state);
        if (it == dist.end() || it->second != cur.dist) continue;
        if (is_goal(cur.state)) {
            std::cout << cur.dist << std::endl;
            return 0;
        }

        // Hallway -> room moves (only move if possible).
        for (int h = 0; h < 11; h++) {
            const int t = get_cell(cur.state, h);
            if (t == 0) continue;
            const int r = t - 1;
            const int door = doors[r];

            // Path must be clear (exclusive).
            if (!hallway_clear(cur.state, h, door)) continue;

            const int top_idx = 11 + r * 2 + 0;
            const int bot_idx = 11 + r * 2 + 1;
            const int top_v = get_cell(cur.state, top_idx);
            const int bot_v = get_cell(cur.state, bot_idx);

            // Room must contain only this type (or empty).
            if ((top_v != 0 && top_v != t) || (bot_v != 0 && bot_v != t)) continue;

            int dest_depth = -1;
            if (bot_v == 0) dest_depth = 1;
            else if (top_v == 0) dest_depth = 0;
            else continue;

            const int dest_idx = 11 + r * 2 + dest_depth;
            const int steps = std::abs(h - door) + (dest_depth + 1);
            const long long cost = cur.dist + energy(t) * steps;
            std::uint64_t ns = cur.state;
            ns = set_cell(ns, h, 0);
            ns = set_cell(ns, dest_idx, t);

            auto jt = dist.find(ns);
            if (jt == dist.end() || cost < jt->second) {
                dist[ns] = cost;
                pq.push({cost, ns});
            }
        }

        // Room -> hallway moves.
        for (int r = 0; r < 4; r++) {
            const int top_idx = 11 + r * 2 + 0;
            const int bot_idx = 11 + r * 2 + 1;
            const int top_v = get_cell(cur.state, top_idx);
            const int bot_v = get_cell(cur.state, bot_idx);
            if (top_v == 0 && bot_v == 0) continue;

            int src_depth = -1;
            int t = 0;
            if (top_v != 0) {
                src_depth = 0;
                t = top_v;
            } else {
                src_depth = 1;
                t = bot_v;
            }

            // If amphipod is already in its correct room and everything below is correct, don't move it.
            if (t == r + 1) {
                if (src_depth == 1) continue;
                if (src_depth == 0 && bot_v == t) continue;
            }

            const int door = doors[r];
            const int steps_out = src_depth + 1; // up to hallway

            for (int hs : hall_spots) {
                if (get_cell(cur.state, hs) != 0) continue;
                if (!hallway_clear(cur.state, hs, door)) continue;

                const int steps = steps_out + std::abs(hs - door);
                const long long cost = cur.dist + energy(t) * steps;
                std::uint64_t ns = cur.state;
                ns = set_cell(ns, 11 + r * 2 + src_depth, 0);
                ns = set_cell(ns, hs, t);

                auto jt = dist.find(ns);
                if (jt == dist.end() || cost < jt->second) {
                    dist[ns] = cost;
                    pq.push({cost, ns});
                }
            }
        }
    }

    return 0;
}

