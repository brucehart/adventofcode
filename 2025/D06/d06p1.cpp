#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Equation {
    std::vector<long long> terms;
    char op = '+';

    long long result() const {
        long long value = (op == '*') ? 1 : 0;
        for (long long term : terms) {
            value = (op == '*') ? value * term : value + term;
        }
        return value;
    }
};

int main() {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;  // skip empty rows
        }
        lines.push_back(line);
    }

    if (lines.size() < 2) {
        return 0;  // need at least one data row and one operator row
    }

    // Last line contains the operators for each column/equation.
    std::stringstream op_stream(lines.back());
    std::vector<char> ops;
    std::string token;
    while (op_stream >> token) {
        ops.push_back(token.empty() ? '+' : token.front());
    }

    const std::size_t cols = ops.size();
    std::vector<Equation> equations(cols);
    for (std::size_t i = 0; i < cols; ++i) {
        equations[i].op = ops[i];
    }

    // All previous lines contain column-oriented terms.
    for (std::size_t row = 0; row + 1 < lines.size(); ++row) {
        std::stringstream row_stream(lines[row]);
        for (std::size_t col = 0; col < cols; ++col) {
            long long value;
            if (!(row_stream >> value)) {
                break;  // stop if this row ran out of values
            }
            equations[col].terms.push_back(value);
        }
    }

    long long total = 0;
    for (const auto& eq : equations) {
        total += eq.result();
    }

    std::cout << total << '\n';
}
