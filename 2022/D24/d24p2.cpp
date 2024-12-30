#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <tuple>
#include <algorithm>

// -----------------------------------------------------------------------------
// This code solves Advent of Code 2022, Day 24, Part 2 ("Blizzard Basin").
//
// We need to traverse from the start (top row open cell) to the goal (bottom
// row open cell), then return to the start, then again go to the goal. We must
// avoid cells occupied by blizzards, which move and wrap around every minute.
//
// We reuse the logic from Part 1 but add a small twist: we perform three BFS
// traversals in sequence, each time continuing from where the previous leg
// ended (i.e., with the correct "time offset" so that the blizzards have moved
// for that many minutes already).
//
// Key steps in the solution:
// 1) Read the grid.
// 2) Find the single open cell in the top row (start) and the single open cell
//    in the bottom row (goal).
// 3) Collect blizzards (position plus direction).
// 4) Compute cycle = lcm(R-2, C-2). Precompute for each t in [0..cycle-1] which
//    cells are blocked by a blizzard (or wall).
// 5) Define a BFS function `solveLeg(...)` that, given a starting position and
//    a starting time offset, returns the earliest time you can arrive at the
//    target position.
// 6) Run that BFS three times in sequence:
//       S -> G  => time T1
//       G -> S  => time T2
//       S -> G  => time T3
//    where each BFS starts at the final time of the previous leg.
//
// 7) Output the final arrival time of the third leg.
//
// This approach is efficient enough for typical puzzle sizes.
//
// -----------------------------------------------------------------------------

// Greatest common divisor
static int gcd(int a, int b) {
    while (b != 0) {
        int tmp = a % b;
        a = b;
        b = tmp;
    }
    return a;
}

// Performs a BFS from (startR, startC) at initialTime to reach (goalR, goalC).
// Returns the earliest time at which we can reach the goal.
int solveLeg(
    int startR, int startC,
    int goalR,  int goalC,
    int initialTime,
    const std::vector<std::vector<bool>>& blocked, // blocked[t mod cycle][r*C + c]
    int R, int C, int cycle
) {
    // We'll keep visited[t mod cycle][r*C + c]. We'll reset it for this BFS.
    std::vector<std::vector<bool>> visited(cycle, std::vector<bool>(R * C, false));

    // We'll use a queue of (r, c, time).
    std::queue<std::tuple<int,int,int>> q;

    // Check if the start is blocked at initialTime. We need to see if (startR,startC)
    // is blocked at t = initialTime mod cycle.
    int startT = initialTime % cycle;
    if (!blocked[startT][startR * C + startC]) {
        visited[startT][startR * C + startC] = true;
        q.push({startR, startC, initialTime});
    }

    // Directions (including "stay")
    const int dR[5] = {0, -1, 1, 0, 0};
    const int dC[5] = {0, 0, 0, -1, 1};

    while (!q.empty()) {
        auto [r, c, m] = q.front();
        q.pop();

        // If we've reached the goal, return the time
        if (r == goalR && c == goalC) {
            return m;
        }

        int nextM = m + 1;
        int nextT = nextM % cycle;

        // Try all 5 moves
        for (int i = 0; i < 5; i++) {
            int nr = r + dR[i];
            int nc = c + dC[i];

            // Boundaries
            if (nr < 0 || nr >= R || nc < 0 || nc >= C) {
                continue;
            }
            // Check if not blocked at the next minute
            if (!blocked[nextT][nr * C + nc]) {
                if (!visited[nextT][nr * C + nc]) {
                    visited[nextT][nr * C + nc] = true;
                    q.push({nr, nc, nextM});
                }
            }
        }
    }

    // If BFS fails (shouldn't happen in valid puzzle input), return something large
    return 1'000'000'000;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Read the entire input
    std::vector<std::string> grid;
    {
        std::string line;
        while (std::getline(std::cin, line)) {
            if (!line.empty()) {
                grid.push_back(line);
            }
        }
    }

    int R = static_cast<int>(grid.size());
    int C = static_cast<int>(grid[0].size());

    // Identify start (top row) and goal (bottom row)
    int startRow = 0, startCol = 0;
    int goalRow = R - 1, goalCol = 0;
    for (int c = 0; c < C; c++) {
        if (grid[0][c] == '.') {
            startRow = 0;
            startCol = c;
        }
        if (grid[R - 1][c] == '.') {
            goalRow = R - 1;
            goalCol = c;
        }
    }

    // Collect blizzards
    // We'll store tuples: (row, col, dr, dc)
    std::vector<std::tuple<int,int,int,int>> blizzards;
    auto getDir = [&](char ch) {
        switch (ch) {
            case '^': return std::make_pair(-1, 0);
            case 'v': return std::make_pair(1, 0);
            case '<': return std::make_pair(0, -1);
            case '>': return std::make_pair(0, 1);
        }
        return std::make_pair(0, 0);
    };

    for (int r = 1; r < R - 1; r++) {
        for (int c = 1; c < C - 1; c++) {
            char ch = grid[r][c];
            if (ch == '^' || ch == 'v' || ch == '<' || ch == '>') {
                auto [dr, dc] = getDir(ch);
                blizzards.push_back({r, c, dr, dc});
            }
        }
    }

    // Compute cycle = lcm(R-2, C-2)
    int cycleRows = R - 2; 
    int cycleCols = C - 2; 
    if (cycleRows < 1 || cycleCols < 1) {
        // Minimal edge case, not expected by puzzle. Just do trivial:
        // We'll assume there's a direct path or it's impossible.
        std::cout << 0 << std::endl;
        return 0;
    }
    int g = gcd(cycleRows, cycleCols);
    int cycle = (cycleRows / g) * cycleCols; 

    // Precompute blocked[t][r*C + c]
    std::vector<std::vector<bool>> blocked(cycle, std::vector<bool>(R * C, false));

    // Mark walls as blocked
    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            if (grid[r][c] == '#') {
                for (int t = 0; t < cycle; t++) {
                    blocked[t][r * C + c] = true;
                }
            }
        }
    }

    // For each blizzard, mark positions at each t
    for (auto &b : blizzards) {
        int br, bc, dr, dc;
        std::tie(br, bc, dr, dc) = b;

        // Convert to interior-based indices for modulo
        int rInt = br - 1;
        int cInt = bc - 1;

        for (int t = 0; t < cycle; t++) {
            int rr = (rInt + dr * t) % cycleRows;
            if (rr < 0) rr += cycleRows;
            int cc = (cInt + dc * t) % cycleCols;
            if (cc < 0) cc += cycleCols;

            int rPos = rr + 1;
            int cPos = cc + 1;
            blocked[t][rPos * C + cPos] = true;
        }
    }

    // We need three legs: 
    //   1) start -> goal
    //   2) goal  -> start
    //   3) start -> goal
    // We'll compute them in sequence.

    // 1) start->goal
    int time1 = solveLeg(startRow, startCol, goalRow, goalCol, 
                         /*initialTime*/ 0,
                         blocked, R, C, cycle);

    // 2) goal->start (use time1 as initial time)
    int time2 = solveLeg(goalRow, goalCol, startRow, startCol,
                         /*initialTime*/ time1,
                         blocked, R, C, cycle);

    // 3) start->goal (use time2 as initial time)
    int time3 = solveLeg(startRow, startCol, goalRow, goalCol,
                         /*initialTime*/ time2,
                         blocked, R, C, cycle);

    // That final time3 is the earliest arrival at the goal after all three legs.
    std::cout << time3 << std::endl;
    return 0;
}

