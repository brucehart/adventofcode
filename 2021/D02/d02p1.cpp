#include <iostream>
#include <string>

int main() {
    long long x_coord = 0;
    long long y_coord = 0;

    std::string direction;
    long long magnitude;
    while (std::cin >> direction >> magnitude) {
        if (direction == "forward") {
            x_coord += magnitude;
        } else if (direction == "down") {
            y_coord += magnitude;
        } else if (direction == "up") {
            y_coord -= magnitude;
        }
    }

    std::cout << x_coord * y_coord << std::endl;
    return 0;
}
