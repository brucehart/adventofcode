#include <iostream>
#include <regex>
#include <string>

int main() {
    std::string input;
    int sum = 0;

    // Read all input into a single string
    std::string line;
    while (std::getline(std::cin, line)) {
        input += line;
    }

    // Remove portions of the string between "don't()" and "do()"
    std::regex dont_do_pattern(R"(don't\(\).*?do\(\))");
    input = std::regex_replace(input, dont_do_pattern, "");

    // Conditionally remove everything from "don't()" to the end of the string
    std::regex dont_to_end_pattern(R"(don't\(\)(?!.*?do\(\)).*$)");
    input = std::regex_replace(input, dont_to_end_pattern, "");

    // Find and process all matches of "mul(...)"
    std::regex mul_pattern(R"(mul\((\d+),(\d+)\))");
    std::sregex_iterator begin(input.begin(), input.end(), mul_pattern);
    std::sregex_iterator end;

    for (auto it = begin; it != end; ++it) {
        std::smatch match = *it;
        int num1 = std::stoi(match[1].str());
        int num2 = std::stoi(match[2].str());
        sum += num1 * num2;
    }

    std::cout << sum << std::endl;

    return 0;
}

