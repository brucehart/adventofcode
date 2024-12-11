#include <iostream>
#include <string>
#include <sstream>

int computeSqFt(int length, int width, int height){
    int lw = length * width;
    int wh = width * height;
    int hl = height * length;    
    int smallest = std::min(lw, std::min(wh, hl));
    return 2 * lw + 2 * wh + 2 * hl + smallest;
}

int main(){
    std::string input;
    int totalSqFt = 0;

    while(std::getline(std::cin, input)){
        std::istringstream iss(input);
        int length, width, height;
        char x;
        iss >> length >> x >> width >> x >> height;
        totalSqFt += computeSqFt(length, width, height);        
    }

    std::cout << totalSqFt << std::endl;
}