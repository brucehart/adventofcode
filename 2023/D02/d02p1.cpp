#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

int main() {
    std::string input;    
    int lineCount = 1;
    std::regex pattern(R"(\s(\d+)\s(green|red|blue))");

    std::unordered_map<std::string, int> max_colors;
    max_colors["red"] = 12;    
    max_colors["green"] = 13;
    max_colors["blue"] = 14;

    int sum = 0;

    while (std::getline(std::cin, input)) {
        std::sregex_iterator begin(input.begin(), input.end(), pattern);
        std::sregex_iterator end;

        bool validGame = true;

        for (auto it = begin; it != end; ++it) {
            std::smatch match = *it;
            int value = std::stoi(match[1]);
            std::string color = match[2];

            if (max_colors[color] < value)
                validGame = false;            

            if (!validGame) break;
        }

        if (validGame) sum += lineCount;
        lineCount++;
    }

    std::cout << sum << std::endl;

    return 0;
}
