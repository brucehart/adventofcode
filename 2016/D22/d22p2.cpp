#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <algorithm>
#include <limits>

struct Node {
    int x, y;
    long long used;
    long long avail;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Read and ignore the two header lines
    std::string line;
    std::getline(std::cin, line);
    std::getline(std::cin, line);

    std::vector<Node> nodes;
    nodes.reserve(10000);

    int max_x = 0, max_y = 0;
    // Parse input
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string name, sizeStr, usedStr, availStr, usePct;
        iss >> name >> sizeStr >> usedStr >> availStr >> usePct;
        // name format: /dev/grid/node-xX-yY
        int x = 0, y = 0;
        {
            // extract numbers after 'x' and 'y'
            std::size_t pos_x = name.find("-x");
            std::size_t pos_y = name.find("-y");
            x = std::stoi(name.substr(pos_x + 2, pos_y - (pos_x + 2)));
            y = std::stoi(name.substr(pos_y + 2));
        }
        long long used = std::stoll(usedStr.substr(0, usedStr.size() - 1));   // drop 'T'
        long long avail = std::stoll(availStr.substr(0, availStr.size() - 1)); // drop 'T'
        nodes.push_back({x, y, used, avail});
        max_x = std::max(max_x, x);
        max_y = std::max(max_y, y);
    }

    // Build grid for quick lookup and identify empty node
    std::vector<std::vector<Node>> grid(max_y + 1, std::vector<Node>(max_x + 1));
    int empty_x = 0, empty_y = 0;
    for (auto const& n : nodes) {
        grid[n.y][n.x] = n;
        if (n.used == 0) {
            empty_x = n.x;
            empty_y = n.y;
        }
    }

    // The goal data starts at top-right corner
    int goal_x = max_x;
    int goal_y = 0;

    // The size of the empty node determines how big a wall can be
    long long empty_size = grid[empty_y][empty_x].avail;

    // Mark walls: nodes whose used > empty_size are impassable
    std::vector<std::vector<bool>> is_wall(max_y + 1, std::vector<bool>(max_x + 1, false));
    for (int y = 0; y <= max_y; ++y) {
        for (int x = 0; x <= max_x; ++x) {
            if (grid[y][x].used > empty_size) {
                is_wall[y][x] = true;
            }
        }
    }

    // BFS to find shortest path from empty to the position left of the goal
    int target_x = goal_x - 1;
    int target_y = goal_y;
    const int INF = std::numeric_limits<int>::max();
    std::vector<std::vector<int>> dist(max_y + 1, std::vector<int>(max_x + 1, INF));
    std::queue<std::pair<int,int>> q;

    dist[empty_y][empty_x] = 0;
    q.push({empty_x, empty_y});

    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};

    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();
        int cd = dist[cy][cx];
        if (cx == target_x && cy == target_y) {
            break;
        }
        for (int i = 0; i < 4; ++i) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];
            if (nx < 0 || ny < 0 || nx > max_x || ny > max_y) continue;
            if (is_wall[ny][nx]) continue;
            if (dist[ny][nx] > cd + 1) {
                dist[ny][nx] = cd + 1;
                q.push({nx, ny});
            }
        }
    }

    int steps_to_goal_adjacent = dist[target_y][target_x];

    // Once the empty is adjacent to the goal, each move of the goal one step left
    // takes 5 moves of the empty + 1 move swapping goal into the empty:
    // total per shift = 5 + 1 = 6, but the final shift into (0,0) is just 1 move.
    // However, a simpler formula derived from examples is:
    // total = steps_to_goal_adjacent + 5 * (goal_x - 1) + 1
    long long total_steps = steps_to_goal_adjacent + 5LL * (goal_x - 1) + 1LL;

    std::cout << total_steps << std::endl;
    return 0;
}

