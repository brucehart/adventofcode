#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// Function to check for vertical reflection
int checkVerticalReflection(const std::vector<std::string>& pattern) {
    int rows = pattern.size();
    int cols = pattern[0].size();
    for (int c = 0; c < cols - 1; ++c) {
        bool reflected = true;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; c - j >= 0 && c + 1 + j < cols; ++j) {
                if (pattern[i][c - j] != pattern[i][c + 1 + j]) {
                    reflected = false;
                    break;
                }
            }
            if (!reflected) break;
        }
        if (reflected) return c + 1;
    }
    return 0;
}

// Function to check for horizontal reflection
int checkHorizontalReflection(const std::vector<std::string>& pattern) {
    int rows = pattern.size();
    int cols = pattern[0].size();
    for (int r = 0; r < rows - 1; ++r) {
        bool reflected = true;
        for (int j = 0; j < cols; ++j) {
            for (int i = 0; r - i >= 0 && r + 1 + i < rows; ++i) {
                if (pattern[r - i][j] != pattern[r + 1 + i][j]) {
                    reflected = false;
                    break;
                }
            }
            if (!reflected) break;
        }
        if (reflected) return r + 1;
    }
    return 0;
}

int main() {
    std::vector<std::string> pattern;
    std::string line;
    int total = 0;

    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            // Process the current pattern
            int vertical = checkVerticalReflection(pattern);
            int horizontal = checkHorizontalReflection(pattern);
            total += vertical + 100 * horizontal;
            pattern.clear();
        } else {
            pattern.push_back(line);
        }
    }

    // Process the last pattern (if any)
    if (!pattern.empty()) {
        int vertical = checkVerticalReflection(pattern);
        int horizontal = checkHorizontalReflection(pattern);
        total += vertical + 100 * horizontal;
    }

    std::cout << total << std::endl;

    return 0;
}
