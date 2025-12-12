#include <iostream>
#include <string>
#include <algorithm>

static bool hits(int vx0, int vy0, int xmin, int xmax, int ymin, int ymax) {
    int x = 0, y = 0;
    int vx = vx0, vy = vy0;

    while (true) {
        x += vx;
        y += vy;
        if (x >= xmin && x <= xmax && y >= ymin && y <= ymax) return true;

        if (vx > 0) vx--;
        else if (vx < 0) vx++;
        vy--;

        if (x > xmax && vx >= 0) return false;
        if (y < ymin && vy < 0) return false;
        if (vx == 0 && x < xmin) return false;
    }
}

int main() {
    std::string s;
    std::getline(std::cin, s);
    if (s.empty()) return 0;

    int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    if (std::sscanf(s.c_str(), "target area: x=%d..%d, y=%d..%d", &x1, &x2, &y1, &y2) != 4) return 0;

    const int xmin = std::min(x1, x2);
    const int xmax = std::max(x1, x2);
    const int ymin = std::min(y1, y2);
    const int ymax = std::max(y1, y2);

    // Bounds:
    // vx: if negative-x targets existed we'd expand this, but AoC 2021 uses positive x.
    // vy: for ymin < 0, the highest useful vy is -ymin - 1; low end is ymin.
    const int vx_lo = 0;
    const int vx_hi = xmax;
    const int vy_lo = ymin;
    const int vy_hi = (ymin < 0) ? (-ymin - 1) : ymax;

    long long cnt = 0;
    for (int vx0 = vx_lo; vx0 <= vx_hi; vx0++) {
        for (int vy0 = vy_lo; vy0 <= vy_hi; vy0++) {
            if (hits(vx0, vy0, xmin, xmax, ymin, ymax)) cnt++;
        }
    }

    std::cout << cnt << std::endl;
    return 0;
}

