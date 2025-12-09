#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

static int to_mask(const std::string& s) {
    int mask = 0;
    for (char c : s) mask |= 1 << (c - 'a');
    return mask;
}

int main() {
    std::string line;
    long long total = 0;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);

        std::vector<int> patterns;
        patterns.reserve(10);
        std::string token;
        while (ss >> token && token != "|") patterns.push_back(to_mask(token));

        std::vector<int> outputs;
        for (int i = 0; i < 4 && ss >> token; ++i) outputs.push_back(to_mask(token));
        if (patterns.size() != 10 || outputs.size() != 4) continue;

        std::unordered_map<int, int> digit_of;
        int mask1 = 0, mask4 = 0, mask7 = 0, mask8 = 0;

        for (int m : patterns) {
            const int bits = __builtin_popcount(m);
            if (bits == 2) {
                digit_of[m] = 1;
                mask1 = m;
            } else if (bits == 3) {
                digit_of[m] = 7;
                mask7 = m;
            } else if (bits == 4) {
                digit_of[m] = 4;
                mask4 = m;
            } else if (bits == 7) {
                digit_of[m] = 8;
                mask8 = m;
            }
        }

        int mask6 = 0;
        for (int m : patterns) {
            if (digit_of.count(m)) continue;
            const int bits = __builtin_popcount(m);
            if (bits == 6) {
                if ((m & mask4) == mask4) {
                    digit_of[m] = 9;
                } else if ((m & mask1) == mask1) {
                    digit_of[m] = 0;
                } else {
                    digit_of[m] = 6;
                    mask6 = m;
                }
            }
        }

        for (int m : patterns) {
            if (digit_of.count(m)) continue;
            const int bits = __builtin_popcount(m);
            if (bits == 5) {
                if ((m & mask1) == mask1) {
                    digit_of[m] = 3;
                } else if (mask6 && (m & mask6) == m) {
                    digit_of[m] = 5;
                } else {
                    digit_of[m] = 2;
                }
            }
        }

        int value = 0;
        for (int m : outputs) {
            value = value * 10 + digit_of[m];
        }
        total += value;
    }

    std::cout << total << std::endl;
    return 0;
}
