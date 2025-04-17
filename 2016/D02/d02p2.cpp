#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int main() {
    // Define the keypad layout.
    // Each string represents a row in a 5x5 grid.
    // Unusable positions are marked with a space.
    // The layout corresponds to:
    //     1
    //   2 3 4
    // 5 6 7 8 9
    //   A B C
    //     D
    const std::vector<std::string> keypad = {
        "  1  ",
        " 234 ",
        "56789",
        " ABC ",
        "  D  "
    };

    // Starting position is at '5', which is located at row 2, column 0.
    int row = 2, col = 0;
    std::string code;
    std::string line;

    // Process each line of instructions.
    while (std::getline(std::cin, line)) {
        // Remove any carriage return characters.
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        // Process each move in the current instruction line.
        for (char move : line) {
            int nextRow = row, nextCol = col;
            switch (move) {
                case 'U': nextRow = row - 1; break;
                case 'D': nextRow = row + 1; break;
                case 'L': nextCol = col - 1; break;
                case 'R': nextCol = col + 1; break;
                default: break;
            }
            // Check if the new position is within bounds and not a space.
            if (nextRow >= 0 && nextRow < static_cast<int>(keypad.size()) &&
                nextCol >= 0 && nextCol < static_cast<int>(keypad[nextRow].size()) &&
                keypad[nextRow][nextCol] != ' ') {
                row = nextRow;
                col = nextCol;
            }
        }
        // After following the moves in the current line, append the key.
        code.push_back(keypad[row][col]);
    }

    // Output the final bathroom code.
    std::cout << code << std::endl;
    return 0;
}

