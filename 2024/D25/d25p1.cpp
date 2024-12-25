#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// Read a single schematic (7 lines of 5 characters each) from std::cin.
// Returns true if successfully read 7 lines, false if EOF or not enough lines.
bool readSchematic(std::vector<std::string>& block) {
    block.clear();
    block.reserve(7);

    int linesRead = 0;
    while (linesRead < 7) {
        std::string line;
        if (!std::getline(std::cin, line)) {
            // EOF reached before reading 7 lines
            return false;
        }
        // Skip completely empty lines
        if (line.empty()) {
            continue;
        }
        // Ensure each line has at least 5 characters (should be exactly 5)
        // but we'll just trust the puzzle input format for correctness.
        block.push_back(line);
        ++linesRead;
    }

    return true;
}

// Convert a lock schematic to pin heights.
//
// According to the puzzle, a lock's top row is fully '#' (index 0 of block),
// and its bottom row is fully '.' (index 6 of block).
// We measure from row 1 down to row 5 in each column,
// stopping at the first '.' encountered.
std::vector<int> parseLock(const std::vector<std::string>& block) {
    std::vector<int> heights(5, 0);

    for (int col = 0; col < 5; ++col) {
        int count = 0;
        // Rows 1..5 (inclusive)
        for (int row = 1; row <= 5; ++row) {
            if (block[row][col] == '#') {
                ++count;
            } else {
                break;
            }
        }
        heights[col] = count;
    }

    return heights;
}

// Convert a key schematic to pin (key) heights.
//
// A key's top row is fully '.' (index 0 of block),
// and its bottom row is fully '#' (index 6).
// We measure from row 5 up to row 1 in each column,
// stopping at the first '.' encountered.
std::vector<int> parseKey(const std::vector<std::string>& block) {
    std::vector<int> heights(5, 0);

    for (int col = 0; col < 5; ++col) {
        int count = 0;
        // Rows 5..1 (descending)
        for (int row = 5; row >= 1; --row) {
            if (block[row][col] == '#') {
                ++count;
            } else {
                break;
            }
        }
        heights[col] = count;
    }

    return heights;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<std::vector<int>> locks;
    std::vector<std::vector<int>> keys;

    // Read schematics until we can't read any more
    while (true) {
        std::vector<std::string> block;
        if (!readSchematic(block)) {
            break; // no more full schematics
        }

        // Check the first line to determine if it's a lock or a key
        // Lock => first line all '#'
        // Key  => first line all '.'
        bool isLock = (std::all_of(block[0].begin(), block[0].end(),
                                   [](char c){ return c == '#'; }));

        if (isLock) {
            locks.push_back(parseLock(block));
        } else {
            keys.push_back(parseKey(block));
        }
    }

    // Now compare each lock with each key to see if they fit.
    // They fit if, for every column, lockHeight + keyHeight <= 5.
    long long fitCount = 0;
    for (const auto& l : locks) {
        for (const auto& k : keys) {
            bool fits = true;
            for (int col = 0; col < 5; ++col) {
                if (l[col] + k[col] > 5) {
                    fits = false;
                    break;
                }
            }
            if (fits) {
                ++fitCount;
            }
        }
    }

    // Output the total number of lock/key pairs that fit
    std::cout << fitCount << std::endl;

    return 0;
}

