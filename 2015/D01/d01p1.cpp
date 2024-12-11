#include <iostream>
#include <string>
#include <algorithm>

int main() {
    std::string data;
    int floor = 0;

    std::getline(std::cin, data);
    std::for_each(data.begin(), data.end(), [&floor](char c) {
        if (c == '(') {
            floor++;
        } else if (c == ')') {
            floor--;
        }
    });

    std::cout << floor << std::endl;
}