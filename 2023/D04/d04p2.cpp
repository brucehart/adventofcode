#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <unordered_map>

int main() {
    std::string line;
    int count = 0;
    std::unordered_map<int, int> cardCopies;

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
       
        int numMatches = 0;

        std::for_each(eval_numbers.begin(), eval_numbers.end(), [&](int number) {            
            if (winning_numbers.find(number) != winning_numbers.end()) {
                numMatches++;
            }            
        });
        
        for(int i = 1; i <= numMatches; i++) {
            cardCopies[card_number+i] += (cardCopies[card_number]+1);
        }

        count += cardCopies[card_number] + 1;
    }
    
    std::cout << count << std::endl;

    return 0;
}
