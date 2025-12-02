#include <iostream>
#include <string>

int main() {
    long long x_coord = 0;
    long long y_coord = 0;
    long long aim = 0;

    std::string direction;
    long long magnitude;
    while (std::cin >> direction >> magnitude) {
        if (direction == "forward") {
            x_coord += magnitude;
            y_coord += magnitude * aim;
        } else if (direction == "down") {
            aim += magnitude;
        } else if (direction == "up") {
            aim -= magnitude;
        }
    }

    std::cout << x_coord * y_coord << std::endl;
    return 0;
}
