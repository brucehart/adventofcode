#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// Function to check for vertical reflection
int checkVerticalReflection(const std::vector<std::string>& pattern, int original = -1) {
    int rows = pattern.size();
    int cols = pattern[0].size();
    for (int c = 0; c < cols - 1; ++c) {
        if (c + 1 == original) continue;
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
int checkHorizontalReflection(const std::vector<std::string>& pattern, int original = -1) {
    int rows = pattern.size();
    int cols = pattern[0].size();
    for (int r = 0; r < rows - 1; ++r) {
        if (r + 1 == original) continue;
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
            int originalVertical = checkVerticalReflection(pattern);
            int originalHorizontal = checkHorizontalReflection(pattern);
            
            bool found = false;
            for(int i = 0; i < pattern.size(); ++i) {
                for (int j = 0; j < pattern[0].size(); ++j) {
                    std::vector<std::string> tempPattern = pattern;
                    tempPattern[i][j] = (tempPattern[i][j] == '.') ? '#' : '.';

                    int newVertical = checkVerticalReflection(tempPattern, originalVertical);
                    int newHorizontal = checkHorizontalReflection(tempPattern, originalHorizontal);

                    if (newVertical > 0 && newVertical != originalVertical) {
                        total += newVertical;
                        found = true;
                        break;
                    }
                    
                    if (newHorizontal > 0 && newHorizontal != originalHorizontal) {
                        total += 100 * newHorizontal;
                        found = true;
                        break;
                    }
                }
                if (found) break;
            }
            
            pattern.clear();
        } else {
            pattern.push_back(line);
        }
    }

    // Process the last pattern (if any)
    if (!pattern.empty()) {
        int originalVertical = checkVerticalReflection(pattern);
        int originalHorizontal = checkHorizontalReflection(pattern);
            
        bool found = false;
        for(int i = 0; i < pattern.size(); ++i) {
            for (int j = 0; j < pattern[0].size(); ++j) {
                std::vector<std::string> tempPattern = pattern;
                tempPattern[i][j] = (tempPattern[i][j] == '.') ? '#' : '.';

                int newVertical = checkVerticalReflection(tempPattern, originalVertical);
                int newHorizontal = checkHorizontalReflection(tempPattern, originalHorizontal);

                if (newVertical > 0 && newVertical != originalVertical) {
                    total += newVertical;
                    found = true;
                    break;
                }
                    
                if (newHorizontal > 0 && newHorizontal != originalHorizontal) {
                    total += 100 * newHorizontal;
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
    }

    std::cout << total << std::endl;

    return 0;
}