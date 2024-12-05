#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <map>

struct Hand{
    static const std::string cardValues;
    std::string cards;
    int bid;

    std::map<char, int> cardCount;

    // Helper function to get the count of a specific card in a const context
    int getCardCount(char c) const {
        auto it = cardCount.find(c);
        return (it != cardCount.end()) ? it->second : 0;
    }

    static int cardValue(char c){
        size_t pos = cardValues.find(c);
        return (pos != std::string::npos) ? static_cast<int>(pos) : -1;
    }

    // Compare function to sort hands
    static bool compare(const Hand& a, const Hand& b){
        if(a.handRank() != b.handRank()){
            return a.handRank() > b.handRank();
        }

        // If ranks are equal, compare the sorted cards
        for (size_t i = 0; i < a.cards.size() && i < b.cards.size(); i++){
            int aVal = cardValue(a.cards[i]);
            int bVal = cardValue(b.cards[i]);
            if(aVal != bVal){
                return aVal > bVal;
            }
        }

        // If still equal, compare bids
        return a.bid > b.bid;
    }

    void countCards(){
        for(char c : cards){
            cardCount[c]++;
        }
        // Sort the cards in descending order based on cardValue
        std::sort(cards.begin(), cards.end(), [&](char a, char b) -> bool {
            return cardValue(a) > cardValue(b);
        });
    }

    bool isFiveOfAKind() const {
        for(auto p : cardCount){
            if(p.first == 'J') continue;
            if(p.second + getCardCount('J') == 5){
                return true;
            }
        }
        return false;
    }

    bool isFourOfAKind() const {
        for(auto p : cardCount){
            if(p.first == 'J') continue;
            if(p.second + getCardCount('J') >= 4){
                return true;
            }
        }
        return false;
    }

    bool isFullHouse() const {
        bool three = false, two = false;
        int availableJ = getCardCount('J');

        // First, try to find a three-of-a-kind
        for(auto p : cardCount){
            if(p.first == 'J') continue;
            if(p.second + availableJ >= 3){
                three = true;
                availableJ -= (3 - p.second > 0) ? (3 - p.second) : 0;
                break;
            }
        }

        // Then, try to find a pair
        for(auto p : cardCount){
            if(p.first == 'J') continue;
            if(p.second + availableJ >= 2){
                two = true;
                availableJ -= (2 - p.second > 0) ? (2 - p.second) : 0;
                break;
            }
        }

        return three && two;
    }

    bool isThreeOfAKind() const {
        for(auto p : cardCount){
            if(p.first == 'J') continue;
            if(p.second + getCardCount('J') >= 3){
                return true;
            }
        }
        return false;
    }

    bool isTwoPair() const {
        int pairs = 0;
        int availableJ = getCardCount('J');

        for(auto p : cardCount){
            if(p.first == 'J') continue;
            if(p.second >= 2){
                pairs++;
            }
            else if(p.second + availableJ >= 2){
                pairs++;
                availableJ -= (2 - p.second);
                if(availableJ < 0) availableJ = 0;
            }
        }
        return pairs >= 2;
    }

    bool isOnePair() const {
        for(auto p : cardCount){
            if(p.first == 'J') continue;
            if(p.second >= 2 || (p.second + getCardCount('J') >= 2)){
                return true;
            }
        }
        return false;
    }

    int handRank() const {
        if(isFiveOfAKind())
            return 6;    
        if(isFourOfAKind())
            return 5;
        if(isFullHouse())
            return 4;
        if(isThreeOfAKind())
            return 3;        
        if(isTwoPair())
            return 2;        
        if(isOnePair())
            return 1;
        
        return 0;
    }
};

// Initialize the static member outside the struct
const std::string Hand::cardValues = "23456789TJQKA";

int main(){
    std::string line;
    std::vector<Hand> hands;

    while(std::getline(std::cin, line)){
        if(line.empty()) continue; // Skip empty lines
        std::istringstream ss(line);
        Hand h;
        ss >> h.cards;
        ss >> h.bid;
        h.countCards();
        hands.push_back(h);
    }

    std::sort(hands.begin(), hands.end(), Hand::compare);

    int handRank = 1;
    unsigned long long totalSum = 0;

    for(const auto& hand : hands){
        totalSum += static_cast<unsigned long long>(hand.bid) * handRank;
        handRank++;
    }

    std::cout << totalSum << std::endl;
}
