#include <iostream>
#include <string>
#include <algorithm>

// Day 18: Like a Rogue
int main() {
    // Read the first row from stdin
    std::string prev;
    std::getline(std::cin, prev);

    const int totalRows = 40;
    const std::size_t width = prev.size();

    // Count safe tiles in the first row
    int totalSafe = static_cast<int>(std::count(prev.begin(), prev.end(), '.'));

    // Buffer for generating each new row
    std::string curr(width, '.');

    for (int row = 1; row < totalRows; ++row) {
        // Determine each tile in the new row
        for (std::size_t i = 0; i < width; ++i) {
            bool left   = (i > 0            && prev[i - 1] == '^');
            bool center = (                 prev[i]     == '^');
            bool right  = (i + 1 < width   && prev[i + 1] == '^');

            // A tile is a trap (^) if one of these patterns matches:
            // 1. left and center are traps, but right is not
            // 2. center and right are traps, but left is not
            // 3. only left is a trap
            // 4. only right is a trap
            if ((left && center && !right)    // pattern 1
                || (center && right && !left) // pattern 2
                || (left && !center && !right)// pattern 3
                || (right && !center && !left)) // pattern 4
            {
                curr[i] = '^';
            } else {
                curr[i] = '.';
            }
        }

        // Count safe tiles in the newly generated row
        totalSafe += static_cast<int>(std::count(curr.begin(), curr.end(), '.'));

        // Prepare for next iteration
        prev.swap(curr);
    }

    // Output the total number of safe tiles after 40 rows
    std::cout << totalSafe << std::endl;
    return 0;
}

