#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::unordered_map<std::string, int> id;
    std::vector<std::string> names;
    std::vector<bool> small;
    std::vector<std::vector<int>> adj;

    auto get_id = [&](const std::string& name) {
        auto it = id.find(name);
        if (it != id.end()) return it->second;
        int idx = names.size();
        id[name] = idx;
        names.push_back(name);
        bool is_small = !name.empty() && std::islower(name[0]);
        small.push_back(is_small);
        adj.emplace_back();
        return idx;
    };

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        const auto dash = line.find('-');
        const std::string a = line.substr(0, dash);
        const std::string b = line.substr(dash + 1);
        const int u = get_id(a);
        const int v = get_id(b);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    if (id.count("start") == 0 || id.count("end") == 0) return 0;
    const int start = id["start"];
    const int goal = id["end"];

    std::vector<int> visited(names.size(), 0);
    long long paths = 0;

    std::function<void(int, bool)> dfs = [&](int u, bool used_double) {
        if (u == goal) {
            paths++;
            return;
        }
        visited[u]++;
        for (int v : adj[u]) {
            if (v == start) continue;
            if (!small[v]) {
                dfs(v, used_double);
            } else if (visited[v] == 0) {
                dfs(v, used_double);
            } else if (!used_double && visited[v] == 1) {
                dfs(v, true);
            }
        }
        visited[u]--;
    };

    dfs(start, false);
    std::cout << paths << std::endl;
    return 0;
}
