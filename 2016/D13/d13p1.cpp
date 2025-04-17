#include <iostream>
#include <queue>
#include <unordered_set>
#include <array>
#include <bit>    // for std::popcount
#include <cstdint>

// Helper to encode a coordinate pair into a single integer key
inline std::uint32_t encode(int x, int y) {
    return (static_cast<std::uint32_t>(x) << 16) | static_cast<std::uint32_t>(y);
}

int main() {
    int favorite;
    std::cin >> favorite;

    // Target coordinates
    const int targetX = 31;
    const int targetY = 39;

    // Lambda to determine if (x,y) is an open space (true) or wall (false)
    auto isOpen = [&](int x, int y) -> bool {
        int v = x*x + 3*x + 2*x*y + y + y*y + favorite;
        // std::popcount requires C++20 (<bit>)
        return (std::popcount(static_cast<unsigned int>(v)) % 2) == 0;
    };

    // 4 possible moves: right, left, down, up
    const std::array<std::pair<int,int>,4> directions = {{
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    }};

    std::queue<std::tuple<int,int,int>> bfs;
    std::unordered_set<std::uint32_t> visited;

    // Start at (1,1) with distance 0
    bfs.emplace(1, 1, 0);
    visited.insert(encode(1,1));

    while (!bfs.empty()) {
        auto [x, y, dist] = bfs.front();
        bfs.pop();

        // Check if we've reached the target
        if (x == targetX && y == targetY) {
            std::cout << dist << std::endl;
            return 0;
        }

        // Explore each neighbor
        for (auto [dx, dy] : directions) {
            int nx = x + dx;
            int ny = y + dy;
            if (nx < 0 || ny < 0) {
                continue; // outside valid range
            }

            auto key = encode(nx, ny);
            if (visited.contains(key)) {
                continue; // already visited
            }

            if (!isOpen(nx, ny)) {
                continue; // it's a wall
            }

            // Mark as visited and add to queue
            visited.insert(key);
            bfs.emplace(nx, ny, dist + 1);
        }
    }

    // If we exhaust the search without finding the target (shouldn't happen)
    std::cout << "No path found" << std::endl;
    return 0;
}

