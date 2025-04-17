#include <iostream>
#include <string>
#include <algorithm>

// The keypad is arranged as follows:
//   1 2 3
//   4 5 6
//   7 8 9
//
// We use a 2D array to represent the keypad. The initial position is at '5',
// which is at row 1, column 1.
int main() {
    // Define the keypad layout.
    const char keypad[3][3] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'}
    };

    // Start at "5", which is row = 1, col = 1.
    int row = 1, col = 1;
    std::string code;  // To accumulate the pressed digits.

    std::string line;
    // Read input line by line until EOF. Each line corresponds to one digit.
    while (std::getline(std::cin, line)) {
        // Remove any carriage return characters that might be present.
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

        // Process each character in the current instruction line.
        for (char move : line) {
            switch (move) {
                case 'U': // Move up if not already at the top row.
                    if (row > 0) {
                        --row;
                    }
                    break;
                case 'D': // Move down if not already at the bottom row.
                    if (row < 2) {
                        ++row;
                    }
                    break;
                case 'L': // Move left if not at the leftmost column.
                    if (col > 0) {
                        --col;
                    }
                    break;
                case 'R': // Move right if not at the rightmost column.
                    if (col < 2) {
                        ++col;
                    }
                    break;
                default:
                    // Ignore any unexpected characters.
                    break;
            }
        }
        // After processing a line, append the current button to the code.
        code.push_back(keypad[row][col]);
    }

    // Output the complete bathroom code followed by a newline.
    std::cout << code << std::endl;
    return 0;
}

