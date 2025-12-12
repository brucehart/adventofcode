#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct ShapeInfo {
    long long area = 0;
    int bbox_w = 0;
    int bbox_h = 0;
};

static bool is_region_line(const std::string& line) {
    const std::size_t colon = line.find(':');
    if (colon == std::string::npos) {
        return false;
    }
    return line.find('x') != std::string::npos;
}

static ShapeInfo compute_shape_info(const std::vector<std::string>& grid) {
    int min_x = 1e9, min_y = 1e9;
    int max_x = -1e9, max_y = -1e9;
    long long area = 0;

    for (int y = 0; y < static_cast<int>(grid.size()); y++) {
        for (int x = 0; x < static_cast<int>(grid[y].size()); x++) {
            if (grid[y][x] != '#') {
                continue;
            }
            area++;
            min_x = std::min(min_x, x);
            min_y = std::min(min_y, y);
            max_x = std::max(max_x, x);
            max_y = std::max(max_y, y);
        }
    }

    ShapeInfo info;
    info.area = area;
    if (area > 0) {
        info.bbox_w = max_x - min_x + 1;
        info.bbox_h = max_y - min_y + 1;
    }
    return info;
}

static bool shape_can_fit(const ShapeInfo& s, int w, int h) {
    if (s.area == 0) {
        return true;
    }
    if (s.bbox_w <= w && s.bbox_h <= h) {
        return true;
    }
    if (s.bbox_h <= w && s.bbox_w <= h) {
        return true;
    }
    return false;
}

int main() {
    std::vector<std::vector<std::string>> shapes_grid;
    std::string line;

    std::string first_region_line;
    bool have_first_region_line = false;

    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }

        if (is_region_line(line)) {
            first_region_line = line;
            have_first_region_line = true;
            break;
        }

        const std::size_t colon = line.find(':');
        if (colon == std::string::npos) {
            continue;
        }

        int idx = 0;
        try {
            idx = std::stoi(line.substr(0, colon));
        } catch (...) {
            continue;
        }

        if (idx < 0) {
            continue;
        }

        if (static_cast<int>(shapes_grid.size()) <= idx) {
            shapes_grid.resize(idx + 1);
        }

        std::vector<std::string> grid;
        while (std::getline(std::cin, line)) {
            if (line.empty()) {
                break;
            }
            grid.push_back(line);
        }

        shapes_grid[idx] = grid;
    }

    std::vector<ShapeInfo> shapes;
    shapes.resize(shapes_grid.size());
    for (int i = 0; i < static_cast<int>(shapes_grid.size()); i++) {
        shapes[i] = compute_shape_info(shapes_grid[i]);
    }

    auto process_region_line = [&](const std::string& s) -> bool {
        const std::size_t colon = s.find(':');
        if (colon == std::string::npos) {
            return false;
        }

        const std::string dims = s.substr(0, colon);
        const std::size_t x_pos = dims.find('x');
        if (x_pos == std::string::npos) {
            return false;
        }

        int w = 0;
        int h = 0;
        try {
            w = std::stoi(dims.substr(0, x_pos));
            h = std::stoi(dims.substr(x_pos + 1));
        } catch (...) {
            return false;
        }

        std::stringstream ss(s.substr(colon + 1));
        std::vector<long long> counts;
        long long v = 0;
        while (ss >> v) {
            counts.push_back(v);
        }

        long long total_area = 0;
        for (int i = 0; i < static_cast<int>(shapes.size()) && i < static_cast<int>(counts.size()); i++) {
            if (counts[i] <= 0) {
                continue;
            }
            if (!shape_can_fit(shapes[i], w, h)) {
                return false;
            }
            total_area += counts[i] * shapes[i].area;
            if (total_area > 1LL * w * h) {
                return false;
            }
        }

        return total_area <= 1LL * w * h;
    };

    long long answer = 0;
    if (have_first_region_line) {
        if (process_region_line(first_region_line)) {
            answer++;
        }
    }

    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }
        if (!is_region_line(line)) {
            continue;
        }
        if (process_region_line(line)) {
            answer++;
        }
    }

    std::cout << answer << std::endl;
    return 0;
}

