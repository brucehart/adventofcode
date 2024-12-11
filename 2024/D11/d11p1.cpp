#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <sstream>
#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::cpp_int;

static const unsigned long long MAX_STEPS = 25;

struct Stone {
    cpp_int value;
    unsigned long long stepsRemaining;

    Stone(const cpp_int& value, unsigned long long stepsRemaining) : value(value), stepsRemaining(stepsRemaining) {}

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
        cpp_int stoneVal(stoneStr);
        stones.emplace_back(stoneVal, MAX_STEPS);
    }

    return stones;
}

// A helper function to remove leading zeros from a numeric string.
// If the string becomes empty, return "0".
std::string normalizeNumericString(const std::string& s) {
    std::size_t pos = s.find_first_not_of('0');
    if (pos == std::string::npos) {
        return "0"; // all zeros or empty means zero
    }
    return s.substr(pos);
}

cpp_int countStones(Stone stone, std::map<Stone, cpp_int>& memo) {
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

    std::string vStr = stone.value.str();

    // If the stone's value has an even number of digits, split it into two halves
    if (vStr.size() % 2 == 0) {
        // Split the string into two halves
        std::string firstHalf = vStr.substr(0, vStr.size() / 2);
        std::string secondHalf = vStr.substr(vStr.size() / 2);

        // Normalize halves to remove leading zeros
        firstHalf = normalizeNumericString(firstHalf);
        secondHalf = normalizeNumericString(secondHalf);

        // Convert the halves back to cpp_int values
        cpp_int firstVal(firstHalf);
        cpp_int secondVal(secondHalf);

        // Create new stones from the halves and decrement the steps
        Stone firstHalfStone(firstVal, stone.stepsRemaining - 1);
        Stone secondHalfStone(secondVal, stone.stepsRemaining - 1);

        // Recursively count the stones for each half and sum the results
        cpp_int result = countStones(firstHalfStone, memo) + countStones(secondHalfStone, memo);
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
    cpp_int count = 0;
    std::vector<Stone> stones = getInput(); 
    std::map<Stone, cpp_int> memo;

    for (auto& stone : stones) {
        count += countStones(stone, memo);
    }

    std::cout << count << std::endl;
    return 0;
}