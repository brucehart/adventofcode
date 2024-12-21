#include <algorithm>
#include <chrono>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <optional>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

// Custom hash function for pair<char, char> to use as a key in unordered_map
struct PairHash {
    std::size_t operator()(const std::pair<char, char>& p) const {
        return std::hash<char>()(p.first) * 31 + std::hash<char>()(p.second);
    }
};

// Function to compute all shortest paths between two characters on a keypad
std::vector<std::string> compute_shortest_paths(
    const std::unordered_map<char, std::vector<std::pair<char, char>>>& adjacency,
    char start,
    char end
) {
    struct State {
        char node;
        std::string path;
        std::set<char> visited;
    };

    std::deque<State> queue;
    queue.push_back(State{start, "", {start}});

    std::vector<std::string> shortest_paths;
    size_t shortest_length = SIZE_MAX;

    while (!queue.empty()) {
        State current = queue.front();
        queue.pop_front();

        if (current.node == end) {
            if (current.path.size() <= shortest_length) {
                if (current.path.size() < shortest_length) {
                    shortest_paths.clear();
                    shortest_length = current.path.size();
                }
                shortest_paths.push_back(current.path);
            }
            continue;
        }

        // Prune paths longer than the shortest found
        if (current.path.size() >= shortest_length) {
            continue;
        }

        auto it = adjacency.find(current.node);
        if (it != adjacency.end()) {
            for (const auto& [neighbor, direction] : it->second) {
                if (current.visited.find(neighbor) == current.visited.end()) {
                    State next_state;
                    next_state.node = neighbor;
                    next_state.path = current.path + std::string(1, direction);
                    next_state.visited = current.visited;
                    next_state.visited.insert(neighbor);
                    queue.push_back(next_state);
                }
            }
        }
    }

    return shortest_paths;
}

// Function to get numeric keypad paths, cached for reuse
const std::unordered_map<std::pair<char, char>, std::vector<std::string>, PairHash>& get_numeric_keypad_paths() {
    static std::unordered_map<std::pair<char, char>, std::vector<std::string>, PairHash> numeric_paths;
    static std::once_flag flag;

    std::call_once(flag, [&]() {
        // Define the numeric keypad adjacency list
        std::unordered_map<char, std::vector<std::pair<char, char>>> numeric_keypad = {
            {'7', {{'4', 'v'}, {'8', '>'}}},
            {'8', {{'5', 'v'}, {'9', '>'}, {'7', '<'}}},
            {'9', {{'6', 'v'}, {'8', '<'}}},
            {'4', {{'1', 'v'}, {'5', '>'}, {'7', '^'}}},
            {'5', {{'2', 'v'}, {'6', '>'}, {'4', '<'}, {'8', '^'}}},
            {'6', {{'3', 'v'}, {'5', '<'}, {'9', '^'}}},
            {'1', {{'2', '>'}, {'4', '^'}}},
            {'2', {{'3', '>'}, {'5', '^'}, {'1', '<'}, {'0', 'v'}}},
            {'0', {{'2', '^'}, {'A', '>'}}},
            {'3', {{'6', '^'}, {'2', '<'}, {'A', 'v'}}},
            {'A', {{'0', '<'}, {'3', '^'}}}
        };

        // Compute all pairs of shortest paths
        for (const auto& from : numeric_keypad) {
            for (const auto& to : numeric_keypad) {
                char a = from.first;
                char b = to.first;
                numeric_paths[{a, b}] = compute_shortest_paths(numeric_keypad, a, b);
            }
        }
    });

    return numeric_paths;
}

// Function to get direction keypad paths, cached for reuse
const std::unordered_map<std::pair<char, char>, std::vector<std::string>, PairHash>& get_direction_keypad_paths() {
    static std::unordered_map<std::pair<char, char>, std::vector<std::string>, PairHash> direction_paths;
    static std::once_flag flag;

    std::call_once(flag, [&]() {
        // Define the direction keypad adjacency list
        std::unordered_map<char, std::vector<std::pair<char, char>>> direction_keypad = {
            {'^', {{'A', '>'}, {'v', 'v'}}},
            {'A', {{'^', '<'}, {'>', 'v'}}},
            {'>', {{'A', '^'}, {'v', '<'}}},
            {'<', {{'v', '>'}}},
            {'v', {{'<', '<'}, {'^', '^'}, {'>', '>'}}}
        };

        // Compute all pairs of shortest paths
        for (const auto& from : direction_keypad) {
            for (const auto& to : direction_keypad) {
                char a = from.first;
                char b = to.first;
                direction_paths[{a, b}] = compute_shortest_paths(direction_keypad, a, b);
            }
        }
    });

    return direction_paths;
}

// Function to trim trailing 'A's from a string
std::string trim_trailing_A(const std::string& str) {
    size_t end = str.find_last_not_of('A');
    return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}

// Function to find the shortest sequence with caching
size_t find_min_sequence(
    const std::string& sequence,
    size_t depth,
    bool is_numeric,
    std::unordered_map<std::string, size_t>& cache_numeric,
    std::unordered_map<std::string, size_t>& cache_direction
) {
    // Construct a unique key for caching
    std::string key = sequence + "|" + std::to_string(depth) + "|" + (is_numeric ? "1" : "0");

    // Choose the appropriate cache
    auto& cache = is_numeric ? cache_numeric : cache_direction;

    // Check if the result is already cached
    auto it = cache.find(key);
    if (it != cache.end()) {
        return it->second;
    }

    // Select the appropriate keypad paths
    const auto& paths = is_numeric ? get_numeric_keypad_paths() : get_direction_keypad_paths();

    size_t total = 0;
    // Prepend 'A' to the sequence
    std::string extended_sequence = "A" + sequence;

    // Iterate through consecutive character pairs
    for (size_t i = 0; i + 1 < extended_sequence.size(); ++i) {
        char from = extended_sequence[i];
        char to = extended_sequence[i + 1];
        auto path_it = paths.find({from, to});
        if (path_it == paths.end()) {
            continue;
        }

        const auto& shortest_paths = path_it->second;
        size_t min_path_length = SIZE_MAX;

        for (const auto& path : shortest_paths) {
            if (depth == 0) {
                min_path_length = std::min(min_path_length, path.size() + 1);
            } else {
                std::string new_path = path + 'A';
                size_t sub_result = find_min_sequence(new_path, depth - 1, false, cache_numeric, cache_direction);
                min_path_length = std::min(min_path_length, sub_result);
            }
        }

        total += min_path_length;
    }

    // Cache the result
    cache[key] = total;
    return total;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Read all input lines
    std::vector<std::string> input_lines;
    std::string line;
    while (std::getline(std::cin, line)) {
        input_lines.emplace_back(line);
    }

    // Initialize caches for numeric and direction sequences
    std::unordered_map<std::string, size_t> cache_numeric;
    std::unordered_map<std::string, size_t> cache_direction;

    size_t total = 0;
    

    for (const auto& raw_line : input_lines) {
        // Trim trailing 'A's and parse the number
        std::string trimmed = trim_trailing_A(raw_line);
        size_t number = trimmed.empty() ? 0 : std::stoul(trimmed);

        // Calculate part 1 with depth 2 and numeric keypad
        size_t seq = find_min_sequence(raw_line, 2, true, cache_numeric, cache_direction);
        total += seq * number;
    }

    // Output the results
    std::cout << total << std::endl;
    

    return 0;
}
