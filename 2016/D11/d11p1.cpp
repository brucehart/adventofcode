#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <unordered_set>
#include <deque>
#include <algorithm>

// State of the elevator and items
struct State {
    int elevator;                   // current floor of the elevator (1-4)
    std::vector<int> chipFloors;    // chipFloors[i] = floor of microchip i
    std::vector<int> genFloors;     // genFloors[i]  = floor of generator i
};

// Generate a canonical key for a state by sorting pairs (gen, chip)
// so that equivalent configurations map to the same key
static std::string canonicalKey(State const& s) {
    int n = s.chipFloors.size();
    std::vector<std::pair<int,int>> pairs;
    pairs.reserve(n);
    for (int i = 0; i < n; ++i) {
        pairs.emplace_back(s.genFloors[i], s.chipFloors[i]);
    }
    std::sort(pairs.begin(), pairs.end());
    std::string key;
    key.push_back('0' + s.elevator);
    for (auto const& p : pairs) {
        key.push_back('0' + p.first);
        key.push_back('0' + p.second);
    }
    return key;
}

// Check if every microchip is safe: a chip must not share a floor
// with any other generator unless its own generator is present
static bool isSafe(State const& s) {
    int n = s.chipFloors.size();
    for (int i = 0; i < n; ++i) {
        if (s.chipFloors[i] != s.genFloors[i]) {
            // chip i is unprotected; check for any generator on its floor
            for (int j = 0; j < n; ++j) {
                if (s.genFloors[j] == s.chipFloors[i]) {
                    return false;
                }
            }
        }
    }
    return true;
}

int main() {
    std::string line;
    std::map<std::string,int> typeIndex;
    std::vector<std::tuple<int,std::string,bool>> rawItems;
    // Read and parse each line of input
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }
        // Determine floor number
        int floor = 0;
        if (line.find("first")   != std::string::npos) floor = 1;
        if (line.find("second")  != std::string::npos) floor = 2;
        if (line.find("third")   != std::string::npos) floor = 3;
        if (line.find("fourth")  != std::string::npos) floor = 4;
        // Remove common punctuation (but keep hyphens)
        for (char& c : line) {
            if (c == ',' || c == '.' || c == ';') {
                c = ' ';
            }
        }
        std::stringstream ss(line);
        std::vector<std::string> tokens;
        std::string tok;
        while (ss >> tok) {
            tokens.push_back(tok);
        }
        // Scan for "generator" and "microchip" tokens
        for (size_t i = 1; i < tokens.size(); ++i) {
            if (tokens[i] == "generator" || tokens[i] == "microchip") {
                std::string typeName = tokens[i - 1];
                bool isGen = (tokens[i] == "generator");
                // Strip "-compatible" suffix if present
                const std::string suffix = "-compatible";
                if (!isGen && typeName.size() > suffix.size()
                    && typeName.substr(typeName.size() - suffix.size()) == suffix)
                {
                    typeName = typeName.substr(0, typeName.size() - suffix.size());
                }
                rawItems.emplace_back(floor, typeName, isGen);
            }
        }
    }
    // Assign indices to each unique element type
    for (auto const& item : rawItems) {
        const std::string& typeName = std::get<1>(item);
        if (typeIndex.find(typeName) == typeIndex.end()) {
            int idx = typeIndex.size();
            typeIndex[typeName] = idx;
        }
    }
    int n = typeIndex.size();
    // Prepare initial state
    State start;
    start.elevator = 1;
    start.chipFloors.assign(n, 0);
    start.genFloors.assign(n, 0);
    for (auto const& item : rawItems) {
        int floor = std::get<0>(item);
        const std::string& typeName = std::get<1>(item);
        bool isGen = std::get<2>(item);
        int idx = typeIndex[typeName];
        if (isGen) {
            start.genFloors[idx] = floor;
        } else {
            start.chipFloors[idx] = floor;
        }
    }
    // BFS to find minimum steps
    std::deque<std::pair<State,int>> queue;
    std::unordered_set<std::string> visited;
    queue.emplace_back(start, 0);
    visited.insert(canonicalKey(start));
    while (!queue.empty()) {
        auto [cur, steps] = queue.front();
        queue.pop_front();
        // Check for goal: all items on floor 4
        bool done = std::all_of(
            cur.chipFloors.begin(), cur.chipFloors.end(),
            [](int f){ return f == 4; })
            && std::all_of(
            cur.genFloors.begin(), cur.genFloors.end(),
            [](int f){ return f == 4; });
        if (done) {
            std::cout << steps << std::endl;
            return 0;
        }
        // Gather item indices on current elevator floor
        std::vector<int> itemsHere;
        for (int i = 0; i < n; ++i) {
            if (cur.chipFloors[i] == cur.elevator) {
                itemsHere.push_back(i * 2 + 1); // odd = chip
            }
            if (cur.genFloors[i] == cur.elevator) {
                itemsHere.push_back(i * 2);     // even = gen
            }
        }
        // Try moving 1 or 2 items
        for (int moveCount = 1; moveCount <= 2; ++moveCount) {
            std::vector<bool> selectMask(itemsHere.size());
            std::fill(selectMask.begin(), selectMask.begin() + moveCount, true);
            do {
                // For each direction: up or down
                for (int dir : {+1, -1}) {
                    int newFloor = cur.elevator + dir;
                    // Skip invalid floors
                    if (newFloor < 1 || newFloor > 4) {
                        continue;
                    }
                    // Prune downward moves if no items below
                    if (dir == -1) {
                        bool anyBelow = false;
                        for (int i = 0; i < n; ++i) {
                            if (cur.chipFloors[i] < cur.elevator
                                || cur.genFloors[i]  < cur.elevator)
                            {
                                anyBelow = true;
                                break;
                            }
                        }
                        if (!anyBelow) {
                            continue;
                        }
                    }
                    // Create new state by moving selected items
                    State next = cur;
                    next.elevator = newFloor;
                    for (size_t idx = 0; idx < itemsHere.size(); ++idx) {
                        if (!selectMask[idx]) {
                            continue;
                        }
                        int code = itemsHere[idx];
                        int itemType = code / 2;
                        if (code % 2 == 0) {
                            next.genFloors[itemType] = newFloor;
                        } else {
                            next.chipFloors[itemType] = newFloor;
                        }
                    }
                    // Enqueue if safe and not visited
                    if (isSafe(next)) {
                        std::string key = canonicalKey(next);
                        if (visited.insert(key).second) {
                            queue.emplace_back(std::move(next), steps + 1);
                        }
                    }
                }
            } while (std::prev_permutation(selectMask.begin(), selectMask.end()));
        }
    }
    // No solution found (should not happen for valid input)
    std::cout << "No solution" << std::endl;
    return 0;
}

