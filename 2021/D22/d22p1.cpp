#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

static int idx(int x, int y, int z) {
    // map [-50,50] -> [0,100]
    const int xi = x + 50;
    const int yi = y + 50;
    const int zi = z + 50;
    return (zi * 101 + yi) * 101 + xi;
}

int main() {
    std::vector<unsigned char> on(101 * 101 * 101, 0);

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        char op[4] = {0};
        int x1 = 0, x2 = 0, y1 = 0, y2 = 0, z1 = 0, z2 = 0;
        if (std::sscanf(line.c_str(), "%3s x=%d..%d,y=%d..%d,z=%d..%d", op, &x1, &x2, &y1, &y2, &z1, &z2) != 7) {
            continue;
        }
        const bool turn_on = (op[1] == 'n'); // "on"

        int cx1 = std::max(x1, -50), cx2 = std::min(x2, 50);
        int cy1 = std::max(y1, -50), cy2 = std::min(y2, 50);
        int cz1 = std::max(z1, -50), cz2 = std::min(z2, 50);
        if (cx1 > cx2 || cy1 > cy2 || cz1 > cz2) continue;

        for (int z = cz1; z <= cz2; z++) {
            for (int y = cy1; y <= cy2; y++) {
                for (int x = cx1; x <= cx2; x++) {
                    on[idx(x, y, z)] = turn_on ? 1 : 0;
                }
            }
        }
    }

    long long cnt = 0;
    for (unsigned char v : on) cnt += v ? 1 : 0;
    std::cout << cnt << std::endl;
    return 0;
}

