#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <algorithm>

int main() {
    std::string line;
    int count = 0;

    // Read lines from stdin
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string token;

        // Read and store the card number
        iss >> token; // "Card"
        iss >> token; // "1:"
        token.pop_back(); // Remove the trailing colon
        int card_number = std::stoi(token);

        // Initialize sets for winning and eval numbers
        std::unordered_set<int> winning_numbers;
        std::vector<int> eval_numbers;

        // Read numbers before and after the '|'
        bool is_winning = true; // Flag to track which set we're reading into
        while (iss >> token) {
            
            if (token == "|") {
                is_winning = false;
                continue;
            }
            
            int number = std::stoi(token);

            if (is_winning)
                winning_numbers.insert(number);
            else
                eval_numbers.push_back(number);
            
        }
       
        int cardValue = 0;

        std::for_each(eval_numbers.begin(), eval_numbers.end(), [&](int number) {            
            if (winning_numbers.find(number) != winning_numbers.end()) {
                cardValue = (cardValue == 0) ? 1 : cardValue * 2;
            }            
        });
        
        count += cardValue;
    }

    std::cout << count << std::endl;

    return 0;
}
