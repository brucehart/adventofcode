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

// Function to calculate the number of ways to make a design
long long countWays(const std::string& design, const std::vector<std::string>& towels) {
    std::vector<long long> dp(design.length() + 1, 0);
    dp[0] = 1;

    for (int i = 0; i < design.length(); ++i) {
        if (dp[i] > 0) {
            for (const std::string& towel : towels) {
                if (i + towel.length() <= design.length() && design.substr(i, towel.length()) == towel) {
                    dp[i + towel.length()] += dp[i];
                }
            }
        }
    }

    return dp[design.length()];
}

int main() {
    std::string line;
    std::getline(std::cin, line);

    // Remove spaces from towel patterns    
    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
    
    std::vector<std::string> towels = splitString(line, ',');

    // Clear the potential delimiter from cin
    std::getline(std::cin, line);
    
    long long totalWays = 0;
    while (std::getline(std::cin, line)) {
        totalWays += countWays(line, towels);
    }

    std::cout << totalWays << std::endl;

    return 0;
}
