#include <iostream>
#include <queue>
#include <unordered_set>
#include <array>
#include <bit>       // for std::popcount
#include <cstdint>

// Encode a pair of coordinates (x, y) into a single 32-bit key
inline std::uint32_t encode(int x, int y) {
    return (static_cast<std::uint32_t>(x) << 16) | static_cast<std::uint32_t>(y);
}

int main() {
    int favorite;
    std::cin >> favorite;

    // Maximum number of steps allowed
    const int maxSteps = 50;

    // Determine if a coordinate (x, y) is an open space (true) or a wall (false)
    auto isOpen = [&](int x, int y) -> bool {
        int value = x*x + 3*x + 2*x*y + y + y*y + favorite;
        // Count bits set to 1; even => open space
        return (std::popcount(static_cast<unsigned int>(value)) % 2) == 0;
    };

    // Possible moves: right, left, down, up
    const std::array<std::pair<int,int>,4> directions = {{
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    }};

    // BFS queue entries: x, y, steps taken to reach this point
    std::queue<std::tuple<int,int,int>> bfs;
    std::unordered_set<std::uint32_t> visited;

    // Start from (1,1) at step count 0
    bfs.emplace(1, 1, 0);
    visited.insert(encode(1, 1));

    // Perform BFS up to maxSteps
    while (!bfs.empty()) {
        auto [x, y, steps] = bfs.front();
        bfs.pop();

        // Do not expand further if we've reached the step limit
        if (steps == maxSteps) {
            continue;
        }

        // Explore neighbors
        for (auto [dx, dy] : directions) {
            int nx = x + dx;
            int ny = y + dy;
            if (nx < 0 || ny < 0) {
                continue;  // ignore out-of-bounds
            }
            std::uint32_t key = encode(nx, ny);
            if (visited.contains(key)) {
                continue;  // already counted
            }
            if (!isOpen(nx, ny)) {
                continue;  // hit a wall
            }
            // Mark as visited and enqueue for further exploration
            visited.insert(key);
            bfs.emplace(nx, ny, steps + 1);
        }
    }

    // Output the total number of reachable locations (including the start)
    std::cout << visited.size() << std::endl;
    return 0;
}

