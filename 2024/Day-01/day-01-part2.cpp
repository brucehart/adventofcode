#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>

int main() {
    std::vector<int> list1;
    std::vector<int> list2;
    std::string line;

    // Read from stdin until EOF
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        int num1, num2;
        if (iss >> num1 >> num2) {
            list1.push_back(num1);
            list2.push_back(num2);
        } else {
            std::cerr << "Invalid input. Each line must contain two numbers separated by whitespace.\n";
        }
    }

    int sum = 0;

    std::for_each(list1.begin(), list1.end(), [&list2, &sum](int num1) {
        sum += std::count(list2.begin(), list2.end(), num1)*num1;
    });

    std::cout << sum << std::endl;

    return 0;
}
