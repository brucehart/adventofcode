#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <cctype>

// Function to check if any symbol is adjacent (within one row and one column) to the number's position
bool isAdjacent(int row, int colStart, int colEnd, const std::vector<std::pair<int, int>>& symbols) {
    for (const auto& [symbolRow, symbolCol] : symbols) {
        // Check if the symbol is within one row and one column of the number's position
        if (symbolRow >= row - 1 && symbolRow <= row + 1 &&
            symbolCol >= colStart - 1 && symbolCol <= colEnd + 1) {
            return true;
        }
    }
    return false;
}

int main() {
    std::vector<std::string> lines;                       // Stores all input lines
    std::vector<std::pair<int, int>> symbols;             // Stores positions of non-digit, non-dot characters
    std::string line;                                     // Temporary variable to store each input line
    long long totalSum = 0;                               // Accumulates the sum of relevant numbers

    // Read all input lines and record positions of non-digit and non-dot characters
    int currentRow = 0;
    while (std::getline(std::cin, line)) {
        for (int col = 0; col < static_cast<int>(line.size()); ++col) {
            char c = line[col];
            if (!std::isdigit(static_cast<unsigned char>(c)) && c != '.') {
                symbols.emplace_back(currentRow, col);
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

                // If the number is adjacent to any symbol, add it to the total sum
                if (isAdjacent(row, startCol, endCol, symbols)) {
                    totalSum += number;
                }
            } else {
                ++col; // Move to the next character if the current one is not a digit
            }
        }
    }

    // Output the final sum
    std::cout << totalSum << std::endl;
    return 0;
}
