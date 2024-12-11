#include <iostream>
#include <string>
#include <algorithm>

int main() {
    std::string data;
    int floor = 0;
    int counter = 1;

    std::getline(std::cin, data);

    for(int i = 0; i < data.size(); i++) {
        if (data[i] == '(') {
            floor++;
        } else if (data[i] == ')') {
            floor--;
        }

        if (floor == -1) {
            std::cout << counter << std::endl;
            return 0;
        }

        counter++;
    }
    
}