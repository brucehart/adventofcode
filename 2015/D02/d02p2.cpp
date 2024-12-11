#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>


int computeRibbonLength(int length, int width, int height){
    std::vector<int> perimeters = {2 * length + 2 * width, 2 * width + 2 * height, 2 * height + 2 * length};
    return *std::min_element(perimeters.begin(), perimeters.end()) + (length * width * height);
}

int main(){
    std::string input;
    int totalLength = 0;

    while(std::getline(std::cin, input)){
        std::istringstream iss(input);
        int length, width, height;
        char x;
        iss >> length >> x >> width >> x >> height;
        totalLength += computeRibbonLength(length, width, height);        
    }

    std::cout << totalLength << std::endl;
}