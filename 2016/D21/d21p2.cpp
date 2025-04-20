#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

// Rotate string s left by k steps
static void rotate_left(std::string &s, int k) {
    int n = s.size();
    k %= n;
    std::rotate(s.begin(), s.begin() + k, s.end());
}

// Rotate string s right by k steps
static void rotate_right(std::string &s, int k) {
    int n = s.size();
    k %= n;
    std::rotate(s.begin(), s.end() - k, s.end());
}

// Perform the "rotate based on position of letter a" operation (forward)
static void rotate_based_on_letter(std::string &s, char a) {
    auto pos = static_cast<int>(std::find(s.begin(), s.end(), a) - s.begin());
    int steps = 1 + pos + (pos >= 4 ? 1 : 0);
    rotate_right(s, steps);
}

int main() {
    // Read all scrambling operations into a list
    std::vector<std::string> ops;
    for (std::string line; std::getline(std::cin, line); ) {
        if (!line.empty()) {
            ops.push_back(line);
        }
    }

    // Start from the scrambled password and undo each operation in reverse order
    std::string s = "fbgdceah";

    for (auto it = ops.rbegin(); it != ops.rend(); ++it) {
        std::istringstream iss(*it);
        std::string token;
        iss >> token;

        if (token == "swap") {
            iss >> token; // "position" or "letter"
            if (token == "position") {
                int x, y;
                iss >> x >> token >> token >> y;
                // Inverse is the same as forward
                std::swap(s[x], s[y]);
            } else {
                char a, b;
                iss >> a >> token >> token >> b;
                // Inverse is the same as forward
                auto pa = std::find(s.begin(), s.end(), a);
                auto pb = std::find(s.begin(), s.end(), b);
                std::iter_swap(pa, pb);
            }

        } else if (token == "rotate") {
            iss >> token; // "left", "right", or "based"
            if (token == "left" || token == "right") {
                int steps;
                iss >> steps;
                // Inverse: left -> right, right -> left
                if (token == "left") {
                    rotate_right(s, steps);
                } else {
                    rotate_left(s, steps);
                }
            } else {
                // "based on position of letter X": invert by brute force
                // skip the words "on position of letter"
                for (int i = 0; i < 4; ++i) {
                    iss >> token;
                }
                char a;
                iss >> a;
                std::string candidate = s;
                int n = s.size();
                // Try each possible left rotation until forward matches current
                for (int k = 0; k < n; ++k) {
                    std::string test = candidate;
                    rotate_left(test, k);
                    // apply forward rotation logic
                    rotate_based_on_letter(test, a);
                    if (test == s) {
                        // Found the correct inverse rotation
                        rotate_left(s, k);
                        break;
                    }
                }
            }

        } else if (token == "reverse") {
            // reverse positions X through Y is its own inverse
            int x, y;
            iss >> token >> x >> token >> y;
            std::reverse(s.begin() + x, s.begin() + y + 1);

        } else if (token == "move") {
            // inverse of "move X to Y" is "move Y to X"
            int x, y;
            iss >> token >> x >> token >> token >> y;
            // move from y back to x
            char c = s[y];
            s.erase(s.begin() + y);
            s.insert(s.begin() + x, c);
        }
    }

    // Output the un-scrambled password
    std::cout << s << std::endl;
    return 0;
}

