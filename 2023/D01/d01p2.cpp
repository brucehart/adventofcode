#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

int main() {
    std::string line;

    // Mapping of digit words to their corresponding integer values
    std::vector<std::pair<std::string, int>> digit_words = {
        {"seven", 7}, {"eight", 8}, {"three", 3}, {"four", 4},
        {"five", 5}, {"six", 6}, {"nine", 9}, {"one", 1}, {"two", 2}, 
        {"0", 0}, {"1", 1}, {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}, 
        {"6", 6}, {"7", 7}, {"8", 8}, {"9", 9}
    };  

    int sum = 0;

    while (std::getline(std::cin, line)) {
        std::vector<int> digits; // To store extracted digits in the order they appear

        for (int i=0; i<line.length();i++)
        {
            for (int j=0; j<digit_words.size(); j++)
            {
                if (line.substr(i, digit_words[j].first.length()) == digit_words[j].first)
                {
                    digits.push_back(digit_words[j].second);
                    break;
                }
            }
        }

        sum += 10 * digits[0] + digits[digits.size() - 1];
    }

    std::cout << sum << std::endl;
    
    return 0;
}
