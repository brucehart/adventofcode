#include <iostream>
#include <string>
#include <algorithm>

int main() {
    // Read the first row of tiles
    std::string prev;
    std::getline(std::cin, prev);

    const int totalRows = 400000;
    const std::size_t width = prev.size();

    // Count safe tiles ('.') in the first row
    long long totalSafe = std::count(prev.begin(), prev.end(), '.');

    // Buffer for generating the next row
    std::string curr(width, '.');

    for (int row = 1; row < totalRows; ++row) {
        for (std::size_t i = 0; i < width; ++i) {
            bool left   = (i > 0          && prev[i - 1] == '^');
            bool center = (                prev[i]     == '^');
            bool right  = (i + 1 < width && prev[i + 1] == '^');

            // Apply trap rules:
            // A tile is a trap (^) if one of these patterns matches:
            // 1. left and center are traps, but right is not
            // 2. center and right are traps, but left is not
            // 3. only left is a trap
            // 4. only right is a trap
            if ((left && center && !right)
                || (center && right && !left)
                || (left && !center && !right)
                || (right && !center && !left)) {
                curr[i] = '^';
            } else {
                curr[i] = '.';
                ++totalSafe;  // count safe tile immediately
            }
        }

        // Prepare for next iteration
        prev.swap(curr);
    }

    // Output the total number of safe tiles after 400000 rows
    std::cout << totalSafe << std::endl;
    return 0;
}

