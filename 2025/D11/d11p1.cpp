#include <iostream>
#include <functional>
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

    std::unordered_map<std::string, long long> memo;
    std::unordered_map<std::string, int> state;

    std::function<long long(const std::string&)> count_paths = [&](const std::string& node) -> long long {
        if (node == "out") {
            return 1;
        }

        auto memo_it = memo.find(node);
        if (memo_it != memo.end()) {
            return memo_it->second;
        }

        if (state[node] == 1) {
            return 0;
        }

        state[node] = 1;
        long long total = 0;
        for (const auto& nxt : edges[node]) {
            total += count_paths(nxt);
        }
        state[node] = 2;
        memo[node] = total;
        return total;
    };

    std::cout << count_paths("you") << std::endl;
    return 0;
}
