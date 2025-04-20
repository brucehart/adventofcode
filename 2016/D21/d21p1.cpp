#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

int main() {
    std::string s = "abcdefgh";
    std::string line;

    // Read each operation from stdin
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }

        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "swap") {
            iss >> token; // "position" or "letter"
            if (token == "position") {
                int x, y;
                iss >> x >> token >> token >> y;
                // Swap characters at positions x and y
                std::swap(s[x], s[y]);
            } else { // "letter"
                char a, b;
                iss >> a >> token >> token >> b;
                // Find each letter and swap them
                auto pa = std::find(s.begin(), s.end(), a);
                auto pb = std::find(s.begin(), s.end(), b);
                std::iter_swap(pa, pb);
            }
        } else if (token == "rotate") {
            iss >> token; // "left", "right", or "based"
            if (token == "left" || token == "right") {
                int steps;
                iss >> steps;
                int n = s.size();
                steps %= n;
                if (token == "left") {
                    // Rotate left by moving first 'steps' to the end
                    std::rotate(s.begin(), s.begin() + steps, s.end());
                } else {
                    // Rotate right by moving last 'steps' to the front
                    std::rotate(s.begin(), s.end() - steps, s.end());
                }
            } else {
                // "based on position of letter X"
                // skip "on position of letter"
                for (int i = 0; i < 4; ++i) iss >> token;
                char a;
                iss >> a;
                int idx = static_cast<int>(std::find(s.begin(), s.end(), a) - s.begin());
                int rotations = 1 + idx + (idx >= 4 ? 1 : 0);
                int n = s.size();
                rotations %= n;
                // Rotate right by 'rotations'
                std::rotate(s.begin(), s.end() - rotations, s.end());
            }
        } else if (token == "reverse") {
            // reverse positions X through Y
            int x, y;
            iss >> token >> x >> token >> y;
            std::reverse(s.begin() + x, s.begin() + y + 1);
        } else if (token == "move") {
            // move position X to position Y
            int x, y;
            iss >> token >> x >> token >> token >> y;
            char c = s[x];
            s.erase(s.begin() + x);
            s.insert(s.begin() + y, c);
        }
    }

    // Output the final scrambled password
    std::cout << s << std::endl;
    return 0;
}

