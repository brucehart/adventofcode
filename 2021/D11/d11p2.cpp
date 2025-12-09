#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (!line.empty()) lines.push_back(line);
    }
    if (lines.empty()) return 0;

    const int rows = lines.size();
    const int cols = lines[0].size();
    const int total_octos = rows * cols;
    std::vector<std::vector<int>> energy(rows, std::vector<int>(cols, 0));
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c) energy[r][c] = lines[r][c] - '0';

    auto in_bounds = [&](int r, int c) { return r >= 0 && r < rows && c >= 0 && c < cols; };

    for (int step = 1;; ++step) {
        for (auto& row : energy)
            for (int& v : row) v++;

        std::vector<std::vector<bool>> flashed(rows, std::vector<bool>(cols, false));
        bool progress = true;
        int flashes = 0;
        while (progress) {
            progress = false;
            for (int r = 0; r < rows; ++r) {
                for (int c = 0; c < cols; ++c) {
                    if (energy[r][c] > 9 && !flashed[r][c]) {
                        flashed[r][c] = true;
                        flashes++;
                        progress = true;

                        for (int dr = -1; dr <= 1; ++dr) {
                            for (int dc = -1; dc <= 1; ++dc) {
                                if (dr == 0 && dc == 0) continue;
                                const int nr = r + dr;
                                const int nc = c + dc;
                                if (in_bounds(nr, nc) && !flashed[nr][nc]) energy[nr][nc]++;
                            }
                        }
                    }
                }
            }
        }

        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                if (flashed[r][c]) energy[r][c] = 0;

        if (flashes == total_octos) {
            std::cout << step << std::endl;
            return 0;
        }
    }
}
