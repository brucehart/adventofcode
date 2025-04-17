#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

int main() {
    std::vector<std::vector<int>> rows;
    std::string line;

    // Read input line by line, parsing each line into three integers.
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }
        std::istringstream iss(line);
        std::vector<int> numbers;
        int num;
        while (iss >> num) {
            numbers.push_back(num);
        }
        // Only add rows that contain exactly three numbers.
        if (numbers.size() == 3) {
            rows.push_back(numbers);
        }
    }

    int validCount = 0;
    // Process the rows in groups of three.
    for (std::size_t i = 0; i + 2 < rows.size(); i += 3) {
        // For each column in the three-row group, form a triangle.
        for (int col = 0; col < 3; ++col) {
            std::vector<int> triangle = { rows[i][col], rows[i+1][col], rows[i+2][col] };
            // Sort the sides to easily check the triangle inequality.
            std::sort(triangle.begin(), triangle.end());
            // A valid triangle has the sum of its two smallest sides greater than the largest side.
            if (triangle[0] + triangle[1] > triangle[2]) {
                ++validCount;
            }
        }
    }

    std::cout << validCount << std::endl;
    return 0;
}

