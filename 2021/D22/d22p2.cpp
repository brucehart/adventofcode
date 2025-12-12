#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

struct Cuboid {
    long long x1, x2;
    long long y1, y2;
    long long z1, z2;
    int sign; // +1 or -1
};

static bool intersect(const Cuboid& a, const Cuboid& b, Cuboid& out) {
    out.x1 = std::max(a.x1, b.x1);
    out.x2 = std::min(a.x2, b.x2);
    out.y1 = std::max(a.y1, b.y1);
    out.y2 = std::min(a.y2, b.y2);
    out.z1 = std::max(a.z1, b.z1);
    out.z2 = std::min(a.z2, b.z2);
    return out.x1 <= out.x2 && out.y1 <= out.y2 && out.z1 <= out.z2;
}

static long long volume(const Cuboid& c) {
    const long long dx = c.x2 - c.x1 + 1;
    const long long dy = c.y2 - c.y1 + 1;
    const long long dz = c.z2 - c.z1 + 1;
    return dx * dy * dz;
}

int main() {
    std::vector<Cuboid> acc;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        char op[4] = {0};
        long long x1 = 0, x2 = 0, y1 = 0, y2 = 0, z1 = 0, z2 = 0;
        if (std::sscanf(line.c_str(), "%3s x=%lld..%lld,y=%lld..%lld,z=%lld..%lld", op, &x1, &x2, &y1, &y2, &z1, &z2) != 7) {
            continue;
        }
        const bool turn_on = (op[1] == 'n'); // "on"
        Cuboid cur{x1, x2, y1, y2, z1, z2, +1};

        std::vector<Cuboid> add;
        add.reserve(acc.size() + 1);
        for (const auto& existing : acc) {
            Cuboid inter{0, 0, 0, 0, 0, 0, 0};
            if (!intersect(existing, cur, inter)) continue;
            inter.sign = -existing.sign;
            add.push_back(inter);
        }
        if (turn_on) add.push_back(cur);
        acc.insert(acc.end(), add.begin(), add.end());
    }

    long long total = 0;
    for (const auto& c : acc) {
        total += static_cast<long long>(c.sign) * volume(c);
    }
    std::cout << total << std::endl;
    return 0;
}
