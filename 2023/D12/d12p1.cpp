#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

struct State {
    int pos;
    int groupIdx;
    int currentGroupSize;
    
    bool operator<(const State& other) const {
        if (pos != other.pos) return pos < other.pos;
        if (groupIdx != other.groupIdx) return groupIdx < other.groupIdx;
        return currentGroupSize < other.currentGroupSize;
    }
};

long long solve(const std::string& springs, const std::vector<int>& groups, 
                State state, std::map<State, long long>& memo) {
    // Check memoization
    if (memo.count(state)) {
        return memo[state];
    }
    
    // Base case: reached end of string
    if (state.pos == springs.size()) {
        // Valid if we've used all groups and current group is complete
        if (state.groupIdx == groups.size() && state.currentGroupSize == 0) return 1;
        // Or if we're on last group and it matches exactly
        if (state.groupIdx == groups.size() - 1 && 
            state.currentGroupSize == groups[state.groupIdx]) return 1;
        return 0;
    }
    
    long long result = 0;
    
    // Try both '.' and '#' for '?' positions
    std::vector<char> choices = (springs[state.pos] == '?' ? 
                               std::vector<char>{'.', '#'} : 
                               std::vector<char>{springs[state.pos]});
                               
    for (char c : choices) {
        if (c == '#') {
            // Extend or start new group
            result += solve(springs, groups, 
                          {state.pos + 1, state.groupIdx, state.currentGroupSize + 1}, 
                          memo);
        } else { // c == '.'
            if (state.currentGroupSize > 0) {
                // End current group
                if (state.groupIdx < groups.size() && 
                    state.currentGroupSize == groups[state.groupIdx]) {
                    result += solve(springs, groups, 
                                  {state.pos + 1, state.groupIdx + 1, 0}, 
                                  memo);
                }
            } else {
                // No current group, just move forward
                result += solve(springs, groups, 
                              {state.pos + 1, state.groupIdx, 0}, 
                              memo);
            }
        }
    }
    
    memo[state] = result;
    return result;
}

long long solveRow(const std::string& springs, const std::vector<int>& groups) {
    std::map<State, long long> memo;
    return solve(springs, groups, {0, 0, 0}, memo);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    long long total = 0;
    std::string line;
    
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        std::string springs;
        iss >> springs;
        
        std::vector<int> groups;
        std::string groupStr;
        iss >> groupStr;
        
        std::replace(groupStr.begin(), groupStr.end(), ',', ' ');
        std::istringstream gss(groupStr);
        int num;
        while (gss >> num) {
            groups.push_back(num);
        }
        
        total += solveRow(springs, groups);
    }
    
    std::cout << total << '\n';
    return 0;
}