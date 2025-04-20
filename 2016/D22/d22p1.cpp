#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

struct Node {
    long long used;
    long long avail;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string line;
    // discard the two header lines
    std::getline(std::cin, line);
    std::getline(std::cin, line);

    std::vector<Node> nodes;
    std::vector<long long> availList;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string name, sizeStr, usedStr, availStr, usePct;
        iss >> name >> sizeStr >> usedStr >> availStr >> usePct;

        long long used  = std::stoll(usedStr.substr(0, usedStr.size() - 1));   // drop 'T'
        long long avail = std::stoll(availStr.substr(0, availStr.size() - 1)); // drop 'T'

        nodes.push_back({used, avail});
        availList.push_back(avail);
    }

    std::sort(availList.begin(), availList.end());           // O(N log N)

    long long viablePairs = 0;
    for (const auto& n : nodes) {
        if (n.used == 0) continue;                           // A must be non‑empty

        // nodes B with avail ≥ n.used
        auto firstOK = std::lower_bound(availList.begin(), availList.end(), n.used);
        long long count = static_cast<long long>(availList.end() - firstOK);

        // If this node itself satisfies avail ≥ used, it was included in count –
        // remove it to avoid pairing with itself.
        if (n.avail >= n.used) --count;

        viablePairs += count;
    }

    std::cout << viablePairs << std::endl;
    return 0;
}
