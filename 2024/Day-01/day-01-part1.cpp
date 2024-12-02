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

    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());

    int sum = 0;
    for (size_t i = 0; i < list1.size(); ++i) {
        sum += std::abs(list1[i] - list2[i]);
    }
    std::cout << sum << std::endl;

    return 0;
}
