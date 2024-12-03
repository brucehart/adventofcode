#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

int main() {
    std::string input;        
    std::regex pattern(R"(\s(\d+)\s(green|red|blue))");

    std::unordered_map<std::string, int> max_colors;  

    int sum = 0;

    while (std::getline(std::cin, input)) {
        std::sregex_iterator begin(input.begin(), input.end(), pattern);
        std::sregex_iterator end;

        int power = 1;        
        max_colors["red"] = 0;    
        max_colors["green"] = 0;
        max_colors["blue"] = 0;

        for (auto it = begin; it != end; ++it) {
            std::smatch match = *it;
            int value = std::stoi(match[1]);
            std::string color = match[2];

            max_colors[color] = std::max(max_colors[color], value);            
        }

        std::for_each(max_colors.begin(), max_colors.end(), [&power](auto& pair) {
            power *= pair.second;
        });

        sum += power;        
    }

    std::cout << sum << std::endl;

    return 0;
}
