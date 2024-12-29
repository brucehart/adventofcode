#include <bits/stdc++.h>

// Custom hash function for std::pair<int, int>
struct pair_hash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        // Combine hashes of the two integers using a prime multiplier
        return std::hash<int>()(p.first) * 100000 + std::hash<int>()(p.second);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Set to store blocked positions (rock '#' or sand 'o')
    std::unordered_set<std::pair<int, int>, pair_hash> blocked;

    std::string line;
    int max_y = INT32_MIN;

    // Read and parse each line of input
    while (std::getline(std::cin, line)) {
        // Split the line by " -> " to get individual points
        std::vector<std::string> tokens;
        size_t pos = 0;
        while ((pos = line.find(" -> ")) != std::string::npos) {
            tokens.emplace_back(line.substr(0, pos));
            line.erase(0, pos + 4);
        }
        tokens.emplace_back(line); // Add the last token

        // Parse all points in the current path
        std::vector<std::pair<int, int>> points;
        for (const auto& token : tokens) {
            size_t comma = token.find(',');
            if (comma == std::string::npos) continue; // Invalid format
            int x = std::stoi(token.substr(0, comma));
            int y = std::stoi(token.substr(comma + 1));
            points.emplace_back(x, y);
            // Update maximum y
            if (y > max_y) {
                max_y = y;
            }
        }

        // Mark all positions between consecutive points as blocked
        for (size_t i = 1; i < points.size(); ++i) {
            int x1 = points[i - 1].first;
            int y1 = points[i - 1].second;
            int x2 = points[i].first;
            int y2 = points[i].second;

            if (x1 == x2) {
                // Vertical line
                int y_start = std::min(y1, y2);
                int y_end = std::max(y1, y2);
                for (int y = y_start; y <= y_end; ++y) {
                    blocked.emplace(std::make_pair(x1, y));
                }
            }
            else if (y1 == y2) {
                // Horizontal line
                int x_start = std::min(x1, x2);
                int x_end = std::max(x1, x2);
                for (int x = x_start; x <= x_end; ++x) {
                    blocked.emplace(std::make_pair(x, y1));
                }
            }
            // Diagonal lines are not expected as per problem description
        }
    }

    // Define the floor level
    const int floor_y = max_y + 2;

    // Source of the sand
    const int source_x = 500;
    const int source_y = 0;

    int sand_count = 0;
    bool source_blocked = false;

    while (!source_blocked) {
        int sand_x = source_x;
        int sand_y = source_y;

        while (true) {
            // If the sand is right above the floor, it comes to rest
            if (sand_y + 1 == floor_y) {
                blocked.emplace(std::make_pair(sand_x, sand_y));
                sand_count++;
                if (sand_x == source_x && sand_y == source_y) {
                    source_blocked = true;
                }
                break;
            }

            // Attempt to move down
            std::pair<int, int> down = {sand_x, sand_y + 1};
            if (blocked.find(down) == blocked.end()) {
                sand_x = down.first;
                sand_y = down.second;
                continue;
            }

            // Attempt to move down-left
            std::pair<int, int> down_left = {sand_x - 1, sand_y + 1};
            if (blocked.find(down_left) == blocked.end()) {
                sand_x = down_left.first;
                sand_y = down_left.second;
                continue;
            }

            // Attempt to move down-right
            std::pair<int, int> down_right = {sand_x + 1, sand_y + 1};
            if (blocked.find(down_right) == blocked.end()) {
                sand_x = down_right.first;
                sand_y = down_right.second;
                continue;
            }

            // Sand comes to rest
            blocked.emplace(std::make_pair(sand_x, sand_y));
            sand_count++;
            if (sand_x == source_x && sand_y == source_y) {
                source_blocked = true;
            }
            break;
        }
    }

    // Output the result
    std::cout << sand_count << std::endl;

    return 0;
}

