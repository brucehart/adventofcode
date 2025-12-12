#include <iostream>
#include <string>
#include <algorithm>

int main() {
    std::string s;
    std::getline(std::cin, s);
    if (s.empty()) return 0;

    int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    // target area: x=79..137, y=-176..-117
    if (std::sscanf(s.c_str(), "target area: x=%d..%d, y=%d..%d", &x1, &x2, &y1, &y2) != 4) return 0;
    (void)x1;
    (void)x2;

    const int ymin = std::min(y1, y2);
    const int ymax = std::max(y1, y2);

    // For the common case where the target is below the origin (ymin < 0),
    // the best initial vy is -ymin - 1, giving max height vy*(vy+1)/2.
    if (ymin < 0) {
        const long long vy = static_cast<long long>(-ymin - 1);
        const long long best = vy * (vy + 1) / 2;
        std::cout << best << std::endl;
        return 0;
    }

    // Fallback for non-negative target y (not expected for AoC 2021 inputs).
    // Search a reasonable band and report the best found.
    long long best = 0;
    for (int vx0 = 0; vx0 <= 500; vx0++) {
        for (int vy0 = 0; vy0 <= 2000; vy0++) {
            int x = 0, y = 0, vx = vx0, vy = vy0;
            int peak = 0;
            for (int step = 0; step < 5000; step++) {
                x += vx;
                y += vy;
                peak = std::max(peak, y);
                if (vx > 0) vx--;
                else if (vx < 0) vx++;
                vy--;
                if (x >= std::min(x1, x2) && x <= std::max(x1, x2) && y >= ymin && y <= ymax) {
                    best = std::max(best, static_cast<long long>(peak));
                    break;
                }
                if (x > std::max(x1, x2) && vx >= 0) break;
                if (y < ymin && vy < 0) break;
            }
        }
    }

    std::cout << best << std::endl;
    return 0;
}

