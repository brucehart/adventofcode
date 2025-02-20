#include <iostream>
#include <string>
#include <vector>

// Checks if the given string segment contains an ABBA pattern.
bool containsABBA(const std::string &segment) {
    // Iterate until there are at least 4 characters remaining.
    for (std::size_t i = 0; i + 3 < segment.size(); ++i) {
        // Check for ABBA: a b b a with a != b.
        if (segment[i] == segment[i + 3] &&
            segment[i + 1] == segment[i + 2] &&
            segment[i] != segment[i + 1]) {
            return true;
        }
    }
    return false;
}

int main() {
    std::string line;
    int tlsSupportedCount = 0;

    // Process each IP address from standard input.
    while (std::getline(std::cin, line)) {
        // Vectors to store segments and whether each is inside hypernet.
        std::vector<std::string> segments;
        std::vector<bool> isHypernet;

        // Parse the line into segments.
        bool insideBrackets = false;
        std::string currentSegment;
        for (char ch : line) {
            if (ch == '[' || ch == ']') {
                // Save the current segment if not empty.
                segments.push_back(currentSegment);
                isHypernet.push_back(insideBrackets);
                currentSegment.clear();
                // Toggle the state if we hit '[' (enter hypernet) or ']' (exit hypernet).
                insideBrackets = (ch == '[');
            } else {
                currentSegment.push_back(ch);
            }
        }
        // Add the last segment.
        segments.push_back(currentSegment);
        isHypernet.push_back(insideBrackets);

        bool hasABBAOutside = false;
        bool hasABBAInside = false;

        // Check each segment for an ABBA pattern.
        for (std::size_t i = 0; i < segments.size(); ++i) {
            if (containsABBA(segments[i])) {
                if (isHypernet[i]) {
                    // An ABBA found within hypernet disqualifies the IP.
                    hasABBAInside = true;
                    break;
                } else {
                    hasABBAOutside = true;
                }
            }
        }

        if (hasABBAOutside && !hasABBAInside) {
            ++tlsSupportedCount;
        }
    }

    // Output the final count of IPs supporting TLS.
    std::cout << tlsSupportedCount << std::endl;
    return 0;
}

