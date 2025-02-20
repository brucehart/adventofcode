#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

// Helper function to check if a 3-character substring is an ABA (first and third character are the same and different from the middle)
bool isABA(const std::string &s, std::size_t i) {
    return i + 2 < s.size() && s[i] == s[i + 2] && s[i] != s[i + 1];
}

int main() {
    std::string line;
    int sslSupportedCount = 0;

    // Process each IP address from standard input.
    while (std::getline(std::cin, line)) {
        // Vectors to hold supernet (outside brackets) and hypernet (inside brackets) segments.
        std::vector<std::string> supernetSegments;
        std::vector<std::string> hypernetSegments;

        bool insideBrackets = false;
        std::string currentSegment;
        for (char ch : line) {
            if (ch == '[' || ch == ']') {
                // Save the segment accumulated so far.
                if (!currentSegment.empty()) {
                    if (insideBrackets) {
                        hypernetSegments.push_back(currentSegment);
                    } else {
                        supernetSegments.push_back(currentSegment);
                    }
                }
                currentSegment.clear();
                // Toggle state: '[' starts a hypernet segment, ']' ends it.
                insideBrackets = (ch == '[');
            } else {
                currentSegment.push_back(ch);
            }
        }
        // Add the final segment.
        if (!currentSegment.empty()) {
            if (insideBrackets) {
                hypernetSegments.push_back(currentSegment);
            } else {
                supernetSegments.push_back(currentSegment);
            }
        }

        // Collect all ABA patterns from supernet segments.
        std::unordered_set<std::string> abaSet;
        for (const auto &segment : supernetSegments) {
            for (std::size_t i = 0; i < segment.size(); ++i) {
                if (isABA(segment, i)) {
                    // Record the ABA pattern.
                    abaSet.insert(segment.substr(i, 3));
                }
            }
        }

        // Collect all BAB patterns from hypernet segments.
        std::unordered_set<std::string> babSet;
        for (const auto &segment : hypernetSegments) {
            for (std::size_t i = 0; i < segment.size(); ++i) {
                if (isABA(segment, i)) {
                    babSet.insert(segment.substr(i, 3));
                }
            }
        }

        // Check for any ABA from supernet that has a corresponding BAB in hypernet.
        bool supportsSSL = false;
        for (const auto &aba : abaSet) {
            // Create the corresponding BAB pattern.
            std::string bab;
            bab.push_back(aba[1]);
            bab.push_back(aba[0]);
            bab.push_back(aba[1]);
            if (babSet.find(bab) != babSet.end()) {
                supportsSSL = true;
                break;
            }
        }
        
        if (supportsSSL) {
            ++sslSupportedCount;
        }
    }

    // Output the final count of IPs supporting SSL.
    std::cout << sslSupportedCount << std::endl;
    return 0;
}

