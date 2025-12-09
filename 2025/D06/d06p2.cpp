#include <cctype>
#include <iostream>
#include <string>
#include <vector>

// Part 2: interpret the worksheet as columns read from right-to-left. Each column
// holds one number made from its top-to-bottom digits. Columns that are entirely
// spaces separate the problems. The operator for a problem is the non-space
// character in its bottom row.
int main() {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(std::cin, line)) {
        lines.push_back(line);
    }

    if (lines.size() < 2) {
        return 0;  // need data rows plus an operator row
    }

    // Pad all rows to the same width so columns align.
    std::size_t width = 0;
    for (const auto& row : lines) {
        width = std::max(width, row.size());
    }
    for (auto& row : lines) {
        if (row.size() < width) {
            row.append(width - row.size(), ' ');
        }
    }

    const std::size_t data_rows = lines.size() - 1;
    const auto& op_row = lines.back();

    // Identify separator columns (all spaces in every row).
    std::vector<bool> is_separator(width, true);
    for (std::size_t col = 0; col < width; ++col) {
        for (const auto& row : lines) {
            if (row[col] != ' ') {
                is_separator[col] = false;
                break;
            }
        }
    }

    long long total = 0;

    // Walk columns from right to left, grouping contiguous non-separator columns
    // into a single problem.
    std::size_t col = width;
    while (col > 0) {
        --col;
        if (is_separator[col]) {
            continue;
        }

        // Find the start of this problem block.
        std::size_t start = col;
        while (start > 0 && !is_separator[start - 1]) {
            --start;
        }

        // Operator is the first non-space character in the operator row within the block.
        char op = '+';
        for (std::size_t c = start; c <= col; ++c) {
            if (op_row[c] != ' ') {
                op = op_row[c];
                break;
            }
        }

        // Collect terms by reading columns right-to-left inside the block.
        std::vector<long long> terms;
        for (std::size_t c = col + 1; c-- > start;) {
            std::string digits;
            for (std::size_t r = 0; r < data_rows; ++r) {
                if (std::isdigit(static_cast<unsigned char>(lines[r][c]))) {
                    digits.push_back(lines[r][c]);
                }
            }
            if (digits.empty()) {
                continue;
            }
            long long value = 0;
            for (char d : digits) {
                value = value * 10 + (d - '0');
            }
            terms.push_back(value);
        }

        long long result = (op == '*') ? 1 : 0;
        for (long long term : terms) {
            result = (op == '*') ? result * term : result + term;
        }
        total += result;

        col = start;  // continue scanning left of this block
    }

    std::cout << total << '\n';
}
