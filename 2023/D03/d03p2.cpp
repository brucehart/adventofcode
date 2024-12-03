#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <cctype>
#include <unordered_map>

// Function to check if any symbol is adjacent (within one row and one column) to the number's position
long long isAdjacent(int row, int colStart, int colEnd, const std::vector<std::pair<int, int>>& gears) {
    for (const auto& [gearRow, gearCol] : gears) {
        // Check if the symbol is within one row and one column of the number's position
        if (gearRow >= row - 1 && gearRow <= row + 1 &&
            gearCol >= colStart - 1 && gearCol <= colEnd + 1) {
            return (gearRow << 16) | gearCol;
        }
    }
    return -1;
}

int main() {
    std::vector<std::string> lines;                       // Stores all input lines
    std::vector<std::pair<int, int>> gears;             // Stores positions of non-digit, non-dot characters
    std::unordered_map<long long, std::vector<int>> gearMap;
    std::string line;                                     // Temporary variable to store each input line
    long long totalSum = 0;                               // Accumulates the sum of relevant numbers

    // Read all input lines and record positions of non-digit and non-dot characters
    int currentRow = 0;
    while (std::getline(std::cin, line)) {
        for (int col = 0; col < static_cast<int>(line.size()); ++col) {
            char c = line[col];
            if (c == '*') {
                gears.emplace_back(currentRow, col);
            }
        }
        lines.push_back(line);
        ++currentRow;
    }

    // Iterate through each line to find and process numbers
    for (int row = 0; row < static_cast<int>(lines.size()); ++row) {
        const std::string& currentLine = lines[row];
        int col = 0;
        while (col < static_cast<int>(currentLine.size())) {
            if (std::isdigit(static_cast<unsigned char>(currentLine[col]))) {
                int number = 0;
                int startCol = col;

                // Extract the full number from the current position
                while (col < static_cast<int>(currentLine.size()) && std::isdigit(static_cast<unsigned char>(currentLine[col]))) {
                    number = number * 10 + (currentLine[col] - '0');
                    ++col;
                }
                int endCol = col - 1;

                long long gearMatch = isAdjacent(row, startCol, endCol, gears);

                // If the number is adjacent to any symbol, add it to the total sum
                if (gearMatch > 0) {
                    gearMap[gearMatch].push_back(number);
                }

            } else {
                ++col; // Move to the next character if the current one is not a digit
            }
        }
    }

    for (const auto& [key, numbers] : gearMap) {
        if (numbers.size() == 2) {
            totalSum += numbers[0] * numbers[1];
        }
    }

    // Output the final sum
    std::cout << totalSum << std::endl;
    return 0;
}
