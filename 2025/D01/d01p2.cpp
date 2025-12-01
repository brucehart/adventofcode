#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>

int main() {
    std::vector<int> rotate;    
    std::string line;
    int total_rotation = 50;
    int password_count = 0;
    const int dial_size = 100;


    // Read from stdin until EOF
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        int num1;
        char dir;
        if (iss >> dir >> num1) {
            if (dir == 'L') {
                rotate.push_back(-num1);
            } else if (dir == 'R') {
                rotate.push_back(num1);
            } else {
                std::cerr << "Invalid direction. Use 'L' for left and 'R' for right.\n";
            }
        } else {
            std::cerr << "Invalid input. Each line must contain a direction and a number.\n";
        }
    }

    for(const int& r : rotate) {       
        if (r > 0) { // rotating right
            int first_hit = (dial_size - (total_rotation % dial_size)) % dial_size;
            if (first_hit == 0) first_hit = dial_size;
            if (r >= first_hit) {
                password_count += 1 + (r - first_hit) / dial_size;
            }
        } else if (r < 0) { // rotating left
            int steps = -r;
            int first_hit = total_rotation % dial_size;
            if (first_hit == 0) first_hit = dial_size;
            if (steps >= first_hit) {
                password_count += 1 + (steps - first_hit) / dial_size;
            }
        }

        total_rotation = (total_rotation + r) % dial_size;
        if (total_rotation < 0) total_rotation += dial_size;
    }

    std::cout << password_count << std::endl;

    return 0;
}
