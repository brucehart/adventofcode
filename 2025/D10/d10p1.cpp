#include <algorithm>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

int main() {
    std::string line;
    long long total_presses = 0;

    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }

        const std::size_t left_bracket = line.find('[');
        const std::size_t right_bracket = line.find(']', left_bracket);
        if (left_bracket == std::string::npos || right_bracket == std::string::npos) {
            continue;
        }

        const std::string target_str = line.substr(left_bracket + 1, right_bracket - left_bracket - 1);
        const int n = static_cast<int>(target_str.size());

        unsigned int target_mask = 0;
        for (int i = 0; i < n; ++i) {
            if (target_str[i] == '#') {
                target_mask |= (1u << i);
            }
        }

        std::vector<unsigned int> buttons;
        std::size_t pos = 0;
        while ((pos = line.find('(', pos)) != std::string::npos) {
            const std::size_t close = line.find(')', pos);
            if (close == std::string::npos) {
                break;
            }

            std::string inside = line.substr(pos + 1, close - pos - 1);
            std::replace(inside.begin(), inside.end(), ',', ' ');
            std::stringstream ss(inside);

            unsigned int mask = 0;
            int idx = 0;
            while (ss >> idx) {
                if (idx >= 0 && idx < n) {
                    mask |= (1u << idx);
                }
            }
            buttons.push_back(mask);
            pos = close + 1;
        }

        const int state_count = 1 << n;
        std::vector<int> dist(state_count, -1);
        std::queue<int> q;
        dist[0] = 0;
        q.push(0);

        while (!q.empty() && dist[static_cast<int>(target_mask)] == -1) {
            const int cur = q.front();
            q.pop();
            const int next_dist = dist[cur] + 1;

            for (const auto mask : buttons) {
                const int nxt = cur ^ static_cast<int>(mask);
                if (dist[nxt] == -1) {
                    dist[nxt] = next_dist;
                    q.push(nxt);
                }
            }
        }

        if (dist[static_cast<int>(target_mask)] != -1) {
            total_presses += dist[static_cast<int>(target_mask)];
        }
    }

    std::cout << total_presses << std::endl;
    return 0;
}
