#include <array>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::unordered_map<std::string, std::vector<std::string>> edges;
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }

        const std::size_t colon = line.find(':');
        if (colon == std::string::npos) {
            continue;
        }

        std::string from = line.substr(0, colon);
        while (!from.empty() && from.back() == ' ') {
            from.pop_back();
        }

        std::string rest = line.substr(colon + 1);
        std::stringstream ss(rest);

        std::string to;
        while (ss >> to) {
            edges[from].push_back(to);
            if (!edges.count(to)) {
                edges[to] = {};
            }
        }

        if (!edges.count(from)) {
            edges[from] = {};
        }
    }

    std::unordered_map<std::string, std::array<long long, 4>> memo;
    std::unordered_map<std::string, std::array<int, 4>> state;

    auto set_bit = [](int mask, const std::string& node) -> int {
        if (node == "dac") {
            mask |= 1;
        }
        if (node == "fft") {
            mask |= 2;
        }
        return mask;
    };

    std::function<long long(const std::string&, int)> count_paths =
        [&](const std::string& node, int mask) -> long long {
            mask = set_bit(mask, node);

            if (node == "out") {
                return mask == 3 ? 1LL : 0LL;
            }

            auto memo_it = memo.find(node);
            if (memo_it != memo.end() && memo_it->second[mask] != -1) {
                return memo_it->second[mask];
            }

            if (memo_it == memo.end()) {
                memo[node].fill(-1);
                state[node].fill(0);
            }

            if (state[node][mask] == 1) {
                return 0;
            }

            state[node][mask] = 1;
            long long total = 0;
            for (const auto& nxt : edges[node]) {
                total += count_paths(nxt, mask);
            }
            state[node][mask] = 2;
            memo[node][mask] = total;
            return total;
        };

    std::cout << count_paths("svr", 0) << std::endl;
    return 0;
}
