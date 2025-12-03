#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int main() {
    std::string batteries;
    long total_joltage = 0;

    while(std::cin >> batteries){        
        auto max_jolts = std::max_element(batteries.begin(), batteries.end()-1);        
        total_joltage += 10*((*max_jolts)-'0');
        max_jolts = std::max_element(max_jolts + 1, batteries.end());
        total_joltage += ((*max_jolts)-'0');
    }

    std::cout << total_joltage << std::endl;
}