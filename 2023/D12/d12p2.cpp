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
    if (memo.count(state)) {
        return memo[state];
    }
    
    if (state.pos == springs.size()) {
        if (state.groupIdx == groups.size() && state.currentGroupSize == 0) return 1;
        if (state.groupIdx == groups.size() - 1 && 
            state.currentGroupSize == groups[state.groupIdx]) return 1;
        return 0;
    }
    
    long long result = 0;
    std::vector<char> choices = (springs[state.pos] == '?' ? 
                               std::vector<char>{'.', '#'} : 
                               std::vector<char>{springs[state.pos]});
                               
    for (char c : choices) {
        if (c == '#') {
            // Check if adding to current group would exceed group size
            if (state.groupIdx < groups.size() && 
                (state.currentGroupSize + 1) <= groups[state.groupIdx]) {
                result += solve(springs, groups, 
                              {state.pos + 1, state.groupIdx, state.currentGroupSize + 1}, 
                              memo);
            }
        } else { // c == '.'
            if (state.currentGroupSize > 0) {
                if (state.groupIdx < groups.size() && 
                    state.currentGroupSize == groups[state.groupIdx]) {
                    result += solve(springs, groups, 
                                  {state.pos + 1, state.groupIdx + 1, 0}, 
                                  memo);
                }
            } else {
                result += solve(springs, groups, 
                              {state.pos + 1, state.groupIdx, 0}, 
                              memo);
            }
        }
    }
    
    memo[state] = result;
    return result;
}

std::string unfoldPattern(const std::string& pattern) {
    std::string result = pattern;
    for (int i = 0; i < 4; i++) {
        result += '?' + pattern;
    }
    return result;
}

std::vector<int> unfoldGroups(const std::vector<int>& groups) {
    std::vector<int> result = groups;
    for (int i = 0; i < 4; i++) {
        result.insert(result.end(), groups.begin(), groups.end());
    }
    return result;
}

long long solveRow(const std::string& springs, const std::vector<int>& groups) {
    std::map<State, long long> memo;
    std::string unfolded_springs = unfoldPattern(springs);
    std::vector<int> unfolded_groups = unfoldGroups(groups);
    return solve(unfolded_springs, unfolded_groups, {0, 0, 0}, memo);
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