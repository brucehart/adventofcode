#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

bool noBadStrings(std::string s){
    std::vector<std::string> badStrings = {"ab", "cd", "pq", "xy"};
    for(auto badString : badStrings){
        if(s.find(badString) != std::string::npos){
            return false;
        }
    }
    return true;
}

bool hasDoubleLetter(std::string s){
    for(int i = 0; i < s.size() - 1; i++){
        if(s[i] == s[i + 1]){
            return true;
        }
    }
    return false;
}

bool hasThreeVowels(std::string s){
    int vowels = 0;
    for(auto c : s){
        if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u'){
            vowels++;
        }
    }
    return vowels >= 3;
}

bool isNice(std::string s){
    return noBadStrings(s) && hasDoubleLetter(s) && hasThreeVowels(s);
}

int main() {
    std::string s;
    int niceStrings = 0;
    while(std::cin >> s){
        if(isNice(s)){
            niceStrings++;
        }
    }
    std::cout << niceStrings << std::endl;
}