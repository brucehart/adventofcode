#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

// Function to split a string by a delimiter
std::vector<std::string> splitString(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to check if a design is possible
bool isDesignPossible(const std::string& design, const std::vector<std::string>& towels) {
    std::vector<bool> dp(design.length() + 1, false);
    dp[0] = true;

    for (int i = 0; i < design.length(); ++i) {
        if (dp[i]) {
            for (const std::string& towel : towels) {
                if (i + towel.length() <= design.length() && design.substr(i, towel.length()) == towel) {
                    dp[i + towel.length()] = true;
                }
            }
        }
    }

    return dp[design.length()];
}

int main() {
    std::string line;
    std::getline(std::cin, line);

    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

    std::vector<std::string> towels = splitString(line, ',');    

    // Clear the potential delimiter from cin
    std::getline(std::cin, line);

    int possibleDesigns = 0;
    while (std::getline(std::cin, line)) {
        if (isDesignPossible(line, towels)) {
            possibleDesigns++;
        }
    }

    std::cout << possibleDesigns << std::endl;

    return 0;
}
