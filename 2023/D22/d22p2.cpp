#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>

// Structure to represent a brick
struct Brick {
    int x1, y1, z1, x2, y2, z2;
    int id;

    Brick(int x1, int y1, int z1, int x2, int y2, int z2, int id) :
        x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2), id(id) {}
};

// Function to parse a line of input into a Brick object
Brick parseBrick(const std::string& line, int id) {
    int x1, y1, z1, x2, y2, z2;
    char comma, tilde;
    std::istringstream iss(line);
    iss >> x1 >> comma >> y1 >> comma >> z1 >> tilde >> x2 >> comma >> y2 >> comma >> z2;
    return Brick(x1, y1, z1, x2, y2, z2, id);
}

// Function to check if two bricks overlap in the x-y plane
bool overlap(const Brick& a, const Brick& b) {
    return std::max(a.x1, b.x1) <= std::min(a.x2, b.x2) &&
           std::max(a.y1, b.y1) <= std::min(a.y2, b.y2);
}

int main() {
    std::vector<Brick> bricks;
    std::string line;
    int id = 0;

    // Read bricks from input
    while (std::getline(std::cin, line)) {
        bricks.emplace_back(parseBrick(line, id++));
    }

    // Sort bricks by their lowest z-coordinate
    std::sort(bricks.begin(), bricks.end(), [](const Brick& a, const Brick& b) {
        return a.z1 < b.z1;
    });

    // Simulate bricks falling
    for (size_t i = 0; i < bricks.size(); ++i) {
        int max_z = 1;
        for (size_t j = 0; j < i; ++j) {
            if (overlap(bricks[i], bricks[j])) {
                max_z = std::max(max_z, bricks[j].z2 + 1);
            }
        }
        bricks[i].z2 -= (bricks[i].z1 - max_z);
        bricks[i].z1 = max_z;
    }

    // Sort again after falling
     std::sort(bricks.begin(), bricks.end(), [](const Brick& a, const Brick& b) {
        return a.z1 < b.z1;
    });

    // Build support relationships
    std::map<int, std::vector<int>> supports;
    std::map<int, std::vector<int>> supported_by;

    for (size_t j = 0; j < bricks.size(); ++j) {
        for (size_t i = j + 1; i < bricks.size(); ++i) {
            if (overlap(bricks[i], bricks[j]) && bricks[i].z1 == bricks[j].z2 + 1) {
                supports[bricks[j].id].push_back(bricks[i].id);
                supported_by[bricks[i].id].push_back(bricks[j].id);
            }
        }
    }

    // Calculate the number of bricks that would fall if each brick were disintegrated
    long long total_fallen_bricks = 0;
    for (size_t i = 0; i < bricks.size(); ++i) {
        std::set<int> fallen_bricks;
        std::vector<int> q = {bricks[i].id};
        fallen_bricks.insert(bricks[i].id);

        int head = 0;
        while (head < q.size()) {
            int current_brick_id = q[head++];
            for (int supported_brick : supports[current_brick_id]) {
                bool all_supporters_fallen = true;
                for (int supporter : supported_by[supported_brick]) {
                    if (fallen_bricks.find(supporter) == fallen_bricks.end()) {
                        all_supporters_fallen = false;
                        break;
                    }
                }
                if (all_supporters_fallen && fallen_bricks.find(supported_brick) == fallen_bricks.end()) {
                    fallen_bricks.insert(supported_brick);
                    q.push_back(supported_brick);
                }
            }
        }
        total_fallen_bricks += fallen_bricks.size() - 1; 
    }

    std::cout << total_fallen_bricks << std::endl;

    return 0;
}
