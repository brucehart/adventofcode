#include <iostream>
#include <regex>
#include <string>

int main() {
    std::string input;
    int sum = 0;
    std::regex pattern(R"(mul\((\d+),(\d+)\))");

    while (std::getline(std::cin, input)) {
        std::sregex_iterator begin(input.begin(), input.end(), pattern);
        std::sregex_iterator end;

        for (auto it = begin; it != end; ++it) {
            std::smatch match = *it;
            int num1 = std::stoi(match[1].str());
            int num2 = std::stoi(match[2].str());
            sum += num1 * num2;
        }
    }

    std::cout << sum << std::endl;

    return 0;
}
