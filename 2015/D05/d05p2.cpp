#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

bool repeatedPair(std::string s){
    for(int i = 0; i < s.size() - 1; i++){
        std::string pair = s.substr(i, 2);
        for(int j = i + 2; j < s.size() - 1; j++){
            if(pair == s.substr(j, 2)){
                return true;
            }
        }
    }
    return false;
}

bool hasRepeatingLetter(std::string s){
    for(int i = 0; i < s.size() - 2; i++){
        if(s[i] == s[i + 2]){
            return true;
        }
    }
    return false;
}

bool isNice(std::string s){
    return repeatedPair(s) && hasRepeatingLetter(s);
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