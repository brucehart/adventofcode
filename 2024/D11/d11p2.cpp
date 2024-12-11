#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <sstream>

using ull = unsigned long long;

static const ull MAX_STEPS = 75;

struct Stone {
    ull value;
    ull stepsRemaining;

    Stone(ull value, ull stepsRemaining) : value(value), stepsRemaining(stepsRemaining) {}

    bool operator<(const Stone& other) const {
        if (stepsRemaining < other.stepsRemaining) return true;
        if (stepsRemaining > other.stepsRemaining) return false;
        return value < other.value;
    }

    bool operator==(const Stone& other) const {
        return (value == other.value && stepsRemaining == other.stepsRemaining);
    }
};

std::vector<Stone> getInput() {
    std::string input;
    std::stringstream ss;
    std::vector<Stone> stones;
    std::string stoneStr;

    std::getline(std::cin, input);
    ss.str(input);

    while (ss >> stoneStr) {
        ull stoneVal = std::stoull(stoneStr);
        stones.emplace_back(stoneVal, MAX_STEPS);
    }

    return stones;
}

ull countStones(Stone stone, std::map<Stone, ull>& memo) {
    // Use a memoization table to store previously computed results for a given stone
    // This helps to avoid redundant calculations and improve efficiency
    auto it = memo.find(stone);
    if (it != memo.end()) {
        return it->second;
    }

    // Base case: If there are no steps remaining, the stone remains as is
    if (stone.stepsRemaining == 0) {
        memo[stone] = 1;
        return 1;
    }

    // If the stone's value is 0, replace it with 1 and decrement the steps
    if (stone.value == 0) {
        stone.value = 1;
        stone.stepsRemaining--;
        memo[stone] = countStones(stone, memo);
        return memo[stone];
    }

    std::string vStr = std::to_string(stone.value);

    // If the stone's value has an even number of digits, split it into two halves
    if (vStr.size() % 2 == 0) {
        // Split the string into two halves
        std::string firstHalf = vStr.substr(0, vStr.size() / 2);
        std::string secondHalf = vStr.substr(vStr.size() / 2);

        // Convert the halves back to ull values
        ull firstVal = std::stoull(firstHalf);
        ull secondVal = std::stoull(secondHalf);

        // Create new stones from the halves and decrement the steps
        Stone firstHalfStone(firstVal, stone.stepsRemaining - 1);
        Stone secondHalfStone(secondVal, stone.stepsRemaining - 1);

        // Recursively count the stones for each half and sum the results
        ull result = countStones(firstHalfStone, memo) + countStones(secondHalfStone, memo);
        memo[stone] = result;
        return result;

    } else {
        // If the stone's value has an odd number of digits, multiply it by 2024
        stone.value *= 2024;
        stone.stepsRemaining--;
        memo[stone] = countStones(stone, memo);
        return memo[stone];
    }
}

int main() {
    ull count = 0;
    std::vector<Stone> stones = getInput(); 
    std::map<Stone, ull> memo;

    for (auto& stone : stones) {
        count += countStones(stone, memo);
    }

    std::cout << count << std::endl;
    return 0;
}
