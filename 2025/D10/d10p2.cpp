#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

static std::vector<int> parse_targets(const std::string& line) {
    std::vector<int> targets;
    const std::size_t l = line.find('{');
    const std::size_t r = line.find('}', l);
    if (l == std::string::npos || r == std::string::npos || r <= l + 1) {
        return targets;
    }
    std::string inside = line.substr(l + 1, r - l - 1);
    std::stringstream ss(inside);
    std::string token;
    while (std::getline(ss, token, ',')) {
        if (!token.empty()) {
            targets.push_back(std::stoi(token));
        }
    }
    return targets;
}

static std::vector<std::vector<int>> parse_buttons(const std::string& line, int counter_count) {
    std::vector<std::vector<int>> buttons;
    std::size_t pos = 0;
    while ((pos = line.find('(', pos)) != std::string::npos) {
        const std::size_t close = line.find(')', pos);
        if (close == std::string::npos) {
            break;
        }
        std::string inside = line.substr(pos + 1, close - pos - 1);
        std::replace(inside.begin(), inside.end(), ',', ' ');
        std::stringstream ss(inside);
        std::vector<int> btn;
        int idx = 0;
        while (ss >> idx) {
            if (idx >= 0 && idx < counter_count) {
                btn.push_back(idx);
            }
        }
        buttons.push_back(std::move(btn));
        pos = close + 1;
    }
    return buttons;
}

static long long solve_machine(const std::vector<int>& target, const std::vector<std::vector<int>>& buttons) {
    const int rows = static_cast<int>(target.size());
    const int cols = static_cast<int>(buttons.size());
    if (rows == 0 || cols == 0) {
        return 0;
    }

    std::vector<std::vector<long double>> mat(rows, std::vector<long double>(cols + 1, 0.0L));
    for (int r = 0; r < rows; ++r) {
        mat[r][cols] = static_cast<long double>(target[r]);
    }
    for (int c = 0; c < cols; ++c) {
        for (int idx : buttons[c]) {
            mat[idx][c] = 1.0L;
        }
    }

    const long double eps = 1e-12L;
    std::vector<int> pivot_col(rows, -1);
    int row = 0;
    for (int col = 0; col < cols && row < rows; ++col) {
        int sel = -1;
        for (int r = row; r < rows; ++r) {
            if (std::abs(mat[r][col]) > eps) {
                sel = r;
                break;
            }
        }
        if (sel == -1) {
            continue;
        }
        std::swap(mat[row], mat[sel]);
        const long double div = mat[row][col];
        for (int c = col; c <= cols; ++c) {
            mat[row][c] /= div;
        }
        for (int r = 0; r < rows; ++r) {
            if (r == row) {
                continue;
            }
            const long double factor = mat[r][col];
            if (std::abs(factor) < eps) {
                continue;
            }
            for (int c = col; c <= cols; ++c) {
                mat[r][c] -= factor * mat[row][c];
            }
        }
        pivot_col[row] = col;
        ++row;
    }
    const int rank = row;

    for (int r = rank; r < rows; ++r) {
        if (std::abs(mat[r][cols]) > eps) {
            return 0;
        }
    }

    std::vector<bool> is_pivot(cols, false);
    for (int r = 0; r < rank; ++r) {
        if (pivot_col[r] != -1) {
            is_pivot[pivot_col[r]] = true;
        }
    }

    std::vector<int> free_cols;
    free_cols.reserve(cols);
    for (int c = 0; c < cols; ++c) {
        if (!is_pivot[c]) {
            free_cols.push_back(c);
        }
    }

    std::vector<int> bounds;
    bounds.reserve(free_cols.size());
    for (int col : free_cols) {
        int bound = std::numeric_limits<int>::max();
        for (int idx : buttons[col]) {
            bound = std::min(bound, target[idx]);
        }
        if (bound == std::numeric_limits<int>::max()) {
            bound = 0;
        }
        bounds.push_back(bound);
    }

    long long best = std::numeric_limits<long long>::max();
    std::vector<long double> free_vals(free_cols.size(), 0.0L);

    const auto dfs = [&](auto&& self, int idx, long long current_sum) -> void {
        if (current_sum >= best) {
            return;
        }
        if (idx == static_cast<int>(free_cols.size())) {
            std::vector<long long> solution(cols, 0);
            for (std::size_t i = 0; i < free_cols.size(); ++i) {
                solution[free_cols[i]] = static_cast<long long>(free_vals[i]);
            }
            for (int r = 0; r < rank; ++r) {
                const int pivot = pivot_col[r];
                long double val = mat[r][cols];
                for (std::size_t i = 0; i < free_cols.size(); ++i) {
                    val -= mat[r][free_cols[i]] * free_vals[i];
                }
                const long long rounded = std::llround(val);
                if (rounded < 0 || std::abs(val - static_cast<long double>(rounded)) > 1e-9L) {
                    return;
                }
                solution[pivot] = rounded;
            }
            long long total = 0;
            for (long long v : solution) {
                total += v;
            }
            if (total < best) {
                best = total;
            }
            return;
        }

        const int bound = bounds[idx];
        for (int v = 0; v <= bound; ++v) {
            if (current_sum + v >= best) {
                break;
            }
            free_vals[idx] = static_cast<long double>(v);
            self(self, idx + 1, current_sum + v);
        }
    };

    dfs(dfs, 0, 0);
    return best == std::numeric_limits<long long>::max() ? 0 : best;
}

int main() {
    std::string line;
    long long total = 0;

    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }
        const std::vector<int> target = parse_targets(line);
        if (target.empty()) {
            continue;
        }
        const std::vector<std::vector<int>> buttons = parse_buttons(line, static_cast<int>(target.size()));
        total += solve_machine(target, buttons);
    }

    std::cout << total << std::endl;
    return 0;
}
