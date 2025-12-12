#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cstdint>
#include <algorithm>

struct Point {
    int x = 0;
    int y = 0;
    int z = 0;
};

static bool operator==(const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

static Point operator+(const Point& a, const Point& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

static Point operator-(const Point& a, const Point& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

struct PointHash {
    std::size_t operator()(const Point& p) const {
        std::uint64_t x = static_cast<std::uint32_t>(p.x);
        std::uint64_t y = static_cast<std::uint32_t>(p.y);
        std::uint64_t z = static_cast<std::uint32_t>(p.z);
        std::uint64_t h = (x << 42) ^ (y << 21) ^ z;
        h ^= (h >> 33);
        h *= 0xff51afd7ed558ccdULL;
        h ^= (h >> 33);
        h *= 0xc4ceb9fe1a85ec53ULL;
        h ^= (h >> 33);
        return static_cast<std::size_t>(h);
    }
};

static Point rot(const Point& p, int r) {
    const int x = p.x, y = p.y, z = p.z;
    switch (r) {
        case 0:  return { x,  y,  z};
        case 1:  return { x, -z,  y};
        case 2:  return { x, -y, -z};
        case 3:  return { x,  z, -y};
        case 4:  return {-x, -y,  z};
        case 5:  return {-x, -z, -y};
        case 6:  return {-x,  y, -z};
        case 7:  return {-x,  z,  y};
        case 8:  return { y,  x, -z};
        case 9:  return { y, -x,  z};
        case 10: return { y,  z,  x};
        case 11: return { y, -z, -x};
        case 12: return {-y,  x,  z};
        case 13: return {-y, -x, -z};
        case 14: return {-y,  z, -x};
        case 15: return {-y, -z,  x};
        case 16: return { z,  x,  y};
        case 17: return { z, -x, -y};
        case 18: return { z,  y, -x};
        case 19: return { z, -y,  x};
        case 20: return {-z,  x, -y};
        case 21: return {-z, -x,  y};
        case 22: return {-z,  y,  x};
        case 23: return {-z, -y, -x};
    }
    return p;
}

static bool align_scanner(
    const std::unordered_set<Point, PointHash>& known,
    const std::vector<Point>& scan,
    int& out_rot,
    Point& out_pos
) {
    std::vector<Point> known_vec;
    known_vec.reserve(known.size());
    for (const auto& p : known) known_vec.push_back(p);

    for (int r = 0; r < 24; r++) {
        std::vector<Point> rotated;
        rotated.reserve(scan.size());
        for (const auto& p : scan) rotated.push_back(rot(p, r));

        std::unordered_map<Point, int, PointHash> cnt;
        cnt.reserve(known_vec.size() * rotated.size());

        for (const auto& a : known_vec) {
            for (const auto& b : rotated) {
                const Point delta = a - b; // scanner position in global coordinates
                int& c = cnt[delta];
                c++;
                if (c >= 12) {
                    out_rot = r;
                    out_pos = delta;
                    return true;
                }
            }
        }
    }
    return false;
}

static long long manhattan(const Point& a, const Point& b) {
    return static_cast<long long>(std::abs(a.x - b.x)) +
           static_cast<long long>(std::abs(a.y - b.y)) +
           static_cast<long long>(std::abs(a.z - b.z));
}

int main() {
    std::vector<std::vector<Point>> scans;
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        if (line.rfind("--- scanner", 0) == 0) {
            scans.emplace_back();
            continue;
        }
        int x = 0, y = 0, z = 0;
        if (std::sscanf(line.c_str(), "%d,%d,%d", &x, &y, &z) == 3) {
            if (scans.empty()) scans.emplace_back();
            scans.back().push_back({x, y, z});
        }
    }
    if (scans.empty()) return 0;

    const int n = static_cast<int>(scans.size());
    std::vector<bool> placed(n, false);
    std::vector<Point> scanner_pos(n, {0, 0, 0});
    std::vector<int> scanner_rot(n, 0);

    std::unordered_set<Point, PointHash> beacons;
    for (const auto& p : scans[0]) beacons.insert(p);
    placed[0] = true;

    while (true) {
        bool progress = false;
        for (int i = 0; i < n; i++) {
            if (placed[i]) continue;

            int r = 0;
            Point pos{0, 0, 0};
            if (!align_scanner(beacons, scans[i], r, pos)) continue;

            for (const auto& p : scans[i]) beacons.insert(rot(p, r) + pos);
            placed[i] = true;
            scanner_pos[i] = pos;
            scanner_rot[i] = r;
            progress = true;
        }
        if (!progress) break;
    }

    long long best = 0;
    for (int i = 0; i < n; i++) {
        if (!placed[i]) continue;
        for (int j = i + 1; j < n; j++) {
            if (!placed[j]) continue;
            best = std::max(best, manhattan(scanner_pos[i], scanner_pos[j]));
        }
    }

    std::cout << best << std::endl;
    return 0;
}

