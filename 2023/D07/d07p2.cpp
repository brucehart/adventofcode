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

    static int cardValue(char c){
        return cardValues.find(c);
    }

    static bool compare(const Hand& a, const Hand& b){        
        if(a.handRank() != b.handRank()){
            return a.handRank() > b.handRank();
        }
        
        for (int i = 0; i < 5; i++){
            if(Hand::cardValue(a.cards[i]) != Hand::cardValue(b.cards[i])){                
                return Hand::cardValue(a.cards[i]) > Hand::cardValue(b.cards[i]);
            }
        }
        
        return true;
    }

    void countCards(){
        for(char c : cards){
            cardCount[c]++;
        }

        std::string cardOrder = Hand::cardValues;
        std::reverse(cardOrder.begin(), cardOrder.end());

        int cardMax = 0;

        for(char c : cardOrder){            
            if(c != 'J' && cardCount[c] > cardMax){                
                cardMax = cardCount[c];
            }
        }        

        for(char c : cardOrder){
            if(c != 'J' && cardCount[c] == cardMax){                
                cardCount[c] += cardCount['J'];
                break;
            }
        }
        
        cardCount['J'] = 0;
    }

    bool isFiveOfAKind() const {
        if (cards == "JJJJJ")
            return true;

        for(auto p : cardCount){
            if(p.second == 5){
                return true;
            }
        }
        return false;
    }

    bool isFourOfAKind() const {
        for(auto p : cardCount){
            if(p.second == 4){
                return true;
            }
        }
        return false;
    }

    bool isFullHouse() const {
        bool three = false, two = false;
        for(auto p : cardCount){
            if(p.second == 3) three = true;
            if(p.second == 2) two = true;
        }
        return three && two;
    }

    bool isThreeOfAKind() const {
        for(auto p : cardCount){
            if(p.second == 3){
                return true;
            }
        }
        return false;
    }

    bool isTwoPair() const {
        int pairs = 0;
        for(auto p : cardCount){
            if(p.second == 2){
                pairs++;
            }
        }
        return pairs >= 2;
    }

    bool isOnePair() const {
        for(auto p : cardCount){
            if(p.second == 2){
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
const std::string Hand::cardValues = "J23456789TQKA";

int main(){
    std::string line;
    std::vector<Hand> hands;

    while(std::getline(std::cin, line)){
        std::istringstream ss(line);
        Hand h;
        ss >> h.cards;
        ss >> h.bid;
        h.countCards();
        hands.push_back(h);
    }

    std::sort(hands.begin(), hands.end(), [](const Hand& a, const Hand& b) -> bool {
        return Hand::compare(b, a); //swap b and a to sort in descending order
    });

    int handRank = 1;
    unsigned long long totalSum = 0;

    for(const auto& hand : hands){       
       totalSum += hand.bid * handRank;
       handRank++;
    }
    
    std::cout << totalSum << std::endl;
}