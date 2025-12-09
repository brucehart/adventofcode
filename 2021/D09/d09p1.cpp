#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> grid;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (!line.empty()) grid.push_back(line);
    }
    if (grid.empty()) return 0;

    const int rows = grid.size();
    const int cols = grid[0].size();
    int risk_sum = 0;

    auto height = [&](int r, int c) { return grid[r][c] - '0'; };
    auto safe = [&](int r, int c) { return r >= 0 && r < rows && c >= 0 && c < cols; };

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            const int h = height(r, c);
            bool low = true;
            if (safe(r - 1, c) && height(r - 1, c) <= h) low = false;
            if (safe(r + 1, c) && height(r + 1, c) <= h) low = false;
            if (safe(r, c - 1) && height(r, c - 1) <= h) low = false;
            if (safe(r, c + 1) && height(r, c + 1) <= h) low = false;

            if (low) risk_sum += h + 1;
        }
    }

    std::cout << risk_sum << std::endl;
    return 0;
}
