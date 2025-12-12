#include <iostream>
#include <string>
#include <cstdint>
#include <algorithm>

struct Wins {
    unsigned long long p1 = 0;
    unsigned long long p2 = 0;
};

static Wins memo[10][10][21][21][2];
static bool seen[10][10][21][21][2];

static int step_pos0(int pos0, int move) {
    return (pos0 + move) % 10;
}

static Wins solve(int p1, int p2, int s1, int s2, int turn) {
    if (s1 >= 21) return {1, 0};
    if (s2 >= 21) return {0, 1};
    if (seen[p1][p2][s1][s2][turn]) return memo[p1][p2][s1][s2][turn];
    seen[p1][p2][s1][s2][turn] = true;

    static const int sums[]  = {3, 4, 5, 6, 7, 8, 9};
    static const int mult[]  = {1, 3, 6, 7, 6, 3, 1};

    Wins res{0, 0};
    for (int i = 0; i < 7; i++) {
        const int move = sums[i];
        const unsigned long long m = static_cast<unsigned long long>(mult[i]);

        if (turn == 0) {
            const int np1 = step_pos0(p1, move);
            const int ns1 = s1 + (np1 + 1);
            const Wins w = solve(np1, p2, ns1, s2, 1);
            res.p1 += m * w.p1;
            res.p2 += m * w.p2;
        } else {
            const int np2 = step_pos0(p2, move);
            const int ns2 = s2 + (np2 + 1);
            const Wins w = solve(p1, np2, s1, ns2, 0);
            res.p1 += m * w.p1;
            res.p2 += m * w.p2;
        }
    }

    memo[p1][p2][s1][s2][turn] = res;
    return res;
}

int main() {
    std::string line;
    int p1 = 0, p2 = 0;
    if (!std::getline(std::cin, line)) return 0;
    std::sscanf(line.c_str(), "Player 1 starting position: %d", &p1);
    if (!std::getline(std::cin, line)) return 0;
    std::sscanf(line.c_str(), "Player 2 starting position: %d", &p2);

    const Wins w = solve(p1 - 1, p2 - 1, 0, 0, 0);
    std::cout << std::max(w.p1, w.p2) << std::endl;
    return 0;
}

