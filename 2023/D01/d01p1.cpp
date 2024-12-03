#include <iostream>
#include <regex>
#include <string>
#include <algorithm>

int main() {
    std::string line;
    std::regex digit_regex("\\d");
    int sum = 0;

    while (std::getline(std::cin, line)) {
        std::smatch match;

        // Find the first digit
        if (std::regex_search(line, match, digit_regex)) {
            sum += 10 * std::stoi(match.str(0));
        }

        // Find the last digit by reversing the string
        auto reverse_line = std::string(line.rbegin(), line.rend());
        if (std::regex_search(reverse_line, match, digit_regex)) {
            sum += std::stoi(match.str(0));
        }
    }

    std::cout << sum << std::endl;

    return 0;
}
