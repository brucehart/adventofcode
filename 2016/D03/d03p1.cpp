#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

int main() {
    int validCount = 0;
    std::string line;

    // Read each line from standard input.
    while (std::getline(std::cin, line)) {
        // Skip empty lines.
        if (line.empty()) {
            continue;
        }

        // Use istringstream to parse the three side lengths from the line.
        std::istringstream iss(line);
        int a, b, c;
        if (!(iss >> a >> b >> c)) {
            // If parsing fails, skip this line.
            continue;
        }

        // Store the sides in a vector and sort them.
        std::vector<int> sides = {a, b, c};
        std::sort(sides.begin(), sides.end());

        // For a valid triangle, the sum of the two smaller sides must be greater than the largest.
        if (sides[0] + sides[1] > sides[2]) {
            ++validCount;
        }
    }

    // Output the total number of valid triangles.
    std::cout << validCount << std::endl;
    return 0;
}

