#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <tuple>
#include <algorithm>
#include <numeric>

// -----------------------------------------------------------------------------
// We solve the "Blizzard Basin" puzzle (Advent of Code 2022, Day 24) as follows:
//
// 1) Read the map from stdin. The map has walls (#) on the perimeter, one open
//    position in the top row (start), and one open position in the bottom row
//    (goal). Each cell can be:
//       '#' - wall
//       '.' - empty ground
//       '^', 'v', '<', '>' - a blizzard, which moves each minute.
//
// 2) Blizzard Movement:
//    - Each blizzard moves one cell in its direction every minute.
//    - If a blizzard moves into a wall on one side, it reappears on the
//      opposite side (wrap-around in that row or column).
//    - Multiple blizzards can occupy the same cell at the same time,
//      but the player cannot be in a cell with a blizzard.
//
// 3) The Player (Expedition) Movement:
//    - The player can move up, down, left, right, or stay in place each minute.
//    - The player's move and all blizzards move simultaneously.
//    - The player cannot move into walls or into a cell occupied by a blizzard.
//
// 4) Approach:
//    - Let R = number of rows, C = number of columns.
//    - Only the interior (excluding walls) matters for blizzard wrapping,
//      which has dimensions (R-2) x (C-2).
//    - Because blizzards wrap around after (R-2) steps vertically or (C-2)
//      steps horizontally, their positions repeat every LCM(R-2, C-2) minutes.
//    - We precompute for each minute (mod cycle) which cells are occupied
//      by blizzards. Then, we run a BFS in the state space:
//        (row, col, t mod cycle).
//      That is, for each minute t, we only need to know t mod cycle to check
//      which cells are blocked.
//
//    - Once we reach the goal position at any minute, that minute count
//      is our answer.
//
// Time Complexity:
//    - The BFS will consider at most R*C*cycle states, where
//      cycle = lcm(R-2, C-2). For typical puzzle sizes, this is feasible.
//
// -----------------------------------------------------------------------------
// Helper function to compute gcd (greatest common divisor) to build LCM.
static int gcd(int a, int b) {
    while (b != 0) {
        int tmp = a % b;
        a = b;
        b = tmp;
    }
    return a;
}

// -----------------------------------------------------------------------------
// Main solver.
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Read the entire input into a vector of strings.
    std::vector<std::string> grid;
    {
        std::string line;
        while (std::getline(std::cin, line)) {
            if (!line.empty()) {
                grid.push_back(line);
            }
        }
    }

    // Basic dimensions
    int R = static_cast<int>(grid.size());
    int C = static_cast<int>(grid[0].size());

    // Locate the start (top row, only '.') and goal (bottom row, only '.').
    // According to the puzzle, there's exactly one open spot in the top and bottom.
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

    // Collect all blizzards: (r, c, dr, dc).
    // dr, dc is the velocity vector depending on the arrow.
    std::vector<std::tuple<int,int,int,int>> blizzards;
    // Directions: '^' -> (-1, 0), 'v' -> (1, 0), '<' -> (0, -1), '>' -> (0, 1)
    auto getDir = [&](char ch) {
        switch (ch) {
            case '^': return std::make_pair(-1, 0);
            case 'v': return std::make_pair(1, 0);
            case '<': return std::make_pair(0, -1);
            case '>': return std::make_pair(0, 1);
        }
        return std::make_pair(0, 0); // Not used for walls/dots
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

    // Compute the cycle = lcm(R-2, C-2).
    int cycleRows = R - 2; // the interior row count
    int cycleCols = C - 2; // the interior column count

    // It's possible one dimension could be 1 if the puzzle was minimal,
    // so we ensure we don't do a zero-lcm scenario. However, puzzle specs
    // usually require at least 2 interior rows or 2 interior columns.
    if (cycleRows < 1 || cycleCols < 1) {
        // Edge case: trivially solve if it's basically 1 row or 1 col.
        // Not expected by the puzzle, but just in case:
        std::cout << (std::abs(goalRow - startRow) + std::abs(goalCol - startCol)) 
                  << std::endl;
        return 0;
    }

    int g = gcd(cycleRows, cycleCols);
    int cycle = (cycleRows / g) * cycleCols; // LCM(R-2, C-2)

    // Precompute for each time t in [0..cycle-1], which cells are blocked by blizzards.
    // We'll store a 2D array of booleans for each t: blocked[t][r][c].
    // But to save memory, we'll store it in a single 2D "blocked[t]" = vector<bool> of size R*C,
    // or more precisely, we'll only check interior cells. We'll store in a
    // 2D array of booleans anyway for clarity.
    std::vector<std::vector<bool>> blocked(cycle, std::vector<bool>(R * C, false));

    // Mark walls as always blocked
    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            if (grid[r][c] == '#') {
                for (int t = 0; t < cycle; t++) {
                    blocked[t][r * C + c] = true;
                }
            }
        }
    }

    // Fill the blocked array for each blizzard position at each time t.
    // Original position of a blizzard is (br, bc). After one minute, it goes
    // to (br+dr, bc+dc) with wrap-around in the interior.
    // In general, at minute t:
    //
    //    row(t) = 1 + ((br - 1) + t*dr) mod (R-2)
    //    col(t) = 1 + ((bc - 1) + t*dc) mod (C-2)
    //
    // We clamp with (mod R-2) or (mod C-2). The +1 is for the walls.
    //
    // We'll do it for t = 0..(cycle-1).
    for (auto &b : blizzards) {
        int br, bc, dr, dc;
        std::tie(br, bc, dr, dc) = b;

        // Convert to 0-based interior index for easier mod:
        int rInt = br - 1; 
        int cInt = bc - 1;

        for (int t = 0; t < cycle; t++) {
            // Where is this blizzard at time t?
            int rr = (rInt + dr * t) % cycleRows;
            if (rr < 0) {
                rr += cycleRows;
            }
            int cc = (cInt + dc * t) % cycleCols;
            if (cc < 0) {
                cc += cycleCols;
            }

            int rPos = rr + 1; 
            int cPos = cc + 1;
            blocked[t][rPos * C + cPos] = true;
        }
    }

    // We run a BFS over (row, col, t mod cycle).
    // We'll store visited[row][col][t mod cycle] to avoid re-processing states.
    // A more memory-efficient approach is to store visited[t mod cycle][row*C+col],
    // or a single 2D array of booleans for each t mod cycle.
    std::vector<std::vector<bool>> visited(cycle, std::vector<bool>(R * C, false));

    // BFS queue: each entry is (row, col, minute).
    // We'll expand in increasing minute order. We don't need a separate distance,
    // because the minute is effectively our BFS layer.
    std::queue<std::tuple<int,int,int>> q;

    // Start state
    if (!blocked[0][startRow * C + startCol]) {
        visited[0][startRow * C + startCol] = true;
        q.push({startRow, startCol, 0});
    }

    // Directions for the expedition: up, down, left, right, plus "stay".
    const int dR[5] = {0, -1, 1, 0, 0};
    const int dC[5] = {0, 0, 0, -1, 1};

    // BFS
    while (!q.empty()) {
        auto [r, c, m] = q.front();
        q.pop();

        // If we've reached goal, print answer and return.
        if (r == goalRow && c == goalCol) {
            std::cout << m << std::endl;
            return 0;
        }

        int nextM = m + 1;
        int nextT = nextM % cycle;

        // Try all 5 moves: stay, up, down, left, right
        for (int i = 0; i < 5; i++) {
            int nr = r + dR[i];
            int nc = c + dC[i];

            // Check boundaries
            if (nr < 0 || nr >= R || nc < 0 || nc >= C) {
                continue;
            }
            // Check if not blocked by wall/blizzard at next minute
            if (!blocked[nextT][nr * C + nc]) {
                // Check if not visited
                if (!visited[nextT][nr * C + nc]) {
                    visited[nextT][nr * C + nc] = true;
                    q.push({nr, nc, nextM});
                }
            }
        }
    }

    // If somehow BFS exhausted and we didn't reach the goal, print something.
    // Puzzle specification suggests there's always a solution, but just in case:
    std::cout << "No path found!" << std::endl;
    return 0;
}
