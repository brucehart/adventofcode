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

struct State {
    std::uint64_t a = 0;
    std::uint64_t b = 0;
};

static bool operator==(const State& x, const State& y) {
    return x.a == y.a && x.b == y.b;
}

struct StateHash {
    std::size_t operator()(const State& s) const {
        std::uint64_t h = s.a ^ (s.b * 0x9e3779b97f4a7c15ULL);
        h ^= (h >> 33);
        h *= 0xff51afd7ed558ccdULL;
        h ^= (h >> 33);
        h *= 0xc4ceb9fe1a85ec53ULL;
        h ^= (h >> 33);
        return static_cast<std::size_t>(h);
    }
};

static int get_cell(const State& s, int idx) {
    if (idx < 21) return static_cast<int>((s.a >> (idx * 3)) & 7ULL);
    idx -= 21;
    return static_cast<int>((s.b >> (idx * 3)) & 7ULL);
}

static State set_cell(State s, int idx, int v) {
    if (idx < 21) {
        const std::uint64_t mask = 7ULL << (idx * 3);
        s.a = (s.a & ~mask) | (static_cast<std::uint64_t>(v) << (idx * 3));
        return s;
    }
    idx -= 21;
    const std::uint64_t mask = 7ULL << (idx * 3);
    s.b = (s.b & ~mask) | (static_cast<std::uint64_t>(v) << (idx * 3));
    return s;
}

static bool is_goal(const State& s) {
    for (int i = 0; i < 11; i++) {
        if (get_cell(s, i) != 0) return false;
    }
    for (int r = 0; r < 4; r++) {
        for (int d = 0; d < 4; d++) {
            const int idx = 11 + r * 4 + d;
            if (get_cell(s, idx) != r + 1) return false;
        }
    }
    return true;
}

static bool hallway_clear(const State& s, int a, int b) {
    if (a > b) std::swap(a, b);
    for (int x = a + 1; x <= b - 1; x++) {
        if (get_cell(s, x) != 0) return false;
    }
    return true;
}

static bool room_all_correct_or_empty(const State& s, int r) {
    const int want = r + 1;
    for (int d = 0; d < 4; d++) {
        const int v = get_cell(s, 11 + r * 4 + d);
        if (v != 0 && v != want) return false;
    }
    return true;
}

static int room_deepest_empty(const State& s, int r) {
    for (int d = 3; d >= 0; d--) {
        if (get_cell(s, 11 + r * 4 + d) == 0) return d;
    }
    return -1;
}

static int room_topmost_occupied(const State& s, int r) {
    for (int d = 0; d < 4; d++) {
        if (get_cell(s, 11 + r * 4 + d) != 0) return d;
    }
    return -1;
}

struct Node {
    long long dist;
    State state;
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

    // If given the part 1 diagram, insert the two extra lines for part 2.
    if (lines.size() == 5) {
        lines.insert(lines.begin() + 3, "  #D#C#B#A#");
        lines.insert(lines.begin() + 4, "  #D#B#A#C#");
    }
    if (lines.size() < 7) return 0;

    int room[4][4];
    for (int r = 0; r < 4; r++) {
        room[r][0] = type_from_char(lines[2][3 + 2 * r]);
        room[r][1] = type_from_char(lines[3][3 + 2 * r]);
        room[r][2] = type_from_char(lines[4][3 + 2 * r]);
        room[r][3] = type_from_char(lines[5][3 + 2 * r]);
    }

    State start{};
    for (int r = 0; r < 4; r++) {
        for (int d = 0; d < 4; d++) {
            start = set_cell(start, 11 + r * 4 + d, room[r][d]);
        }
    }

    static const int doors[4] = {2, 4, 6, 8};
    static const int hall_spots[7] = {0, 1, 3, 5, 7, 9, 10};

    std::priority_queue<Node, std::vector<Node>, Cmp> pq;
    std::unordered_map<State, long long, StateHash> dist;
    dist.reserve(1500000);

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

        // Hallway -> room.
        for (int h = 0; h < 11; h++) {
            const int t = get_cell(cur.state, h);
            if (t == 0) continue;
            const int r = t - 1;
            const int door = doors[r];
            if (!hallway_clear(cur.state, h, door)) continue;
            if (!room_all_correct_or_empty(cur.state, r)) continue;
            const int dest_depth = room_deepest_empty(cur.state, r);
            if (dest_depth < 0) continue;

            const int dest_idx = 11 + r * 4 + dest_depth;
            const int steps = std::abs(h - door) + (dest_depth + 1);
            const long long cost = cur.dist + energy(t) * steps;
            State ns = cur.state;
            ns = set_cell(ns, h, 0);
            ns = set_cell(ns, dest_idx, t);

            auto jt = dist.find(ns);
            if (jt == dist.end() || cost < jt->second) {
                dist[ns] = cost;
                pq.push({cost, ns});
            }
        }

        // Room -> hallway.
        for (int r = 0; r < 4; r++) {
            if (room_all_correct_or_empty(cur.state, r)) continue;
            const int src_depth = room_topmost_occupied(cur.state, r);
            if (src_depth < 0) continue;
            const int src_idx = 11 + r * 4 + src_depth;
            const int t = get_cell(cur.state, src_idx);
            const int door = doors[r];
            const int steps_out = src_depth + 1;

            for (int hs : hall_spots) {
                if (get_cell(cur.state, hs) != 0) continue;
                if (!hallway_clear(cur.state, hs, door)) continue;

                const int steps = steps_out + std::abs(hs - door);
                const long long cost = cur.dist + energy(t) * steps;
                State ns = cur.state;
                ns = set_cell(ns, src_idx, 0);
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

