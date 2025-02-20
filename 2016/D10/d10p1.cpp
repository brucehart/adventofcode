#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <regex>
#include <algorithm>

int main() {
    std::map<int, std::vector<int>> bots;
    std::map<int, int> outputs;
    std::map<int, std::pair<int, int>> bot_instructions;

    std::regex value_regex(R"(value (\d+) goes to bot (\d+))");
    std::regex bot_regex(R"(bot (\d+) gives low to (bot|output) (\d+) and high to (bot|output) (\d+))");

    std::string line;
    while (std::getline(std::cin, line)) {
        std::smatch match;
        if (std::regex_match(line, match, value_regex)) {
            int value = std::stoi(match[1]);
            int bot_id = std::stoi(match[2]);
            bots[bot_id].push_back(value);
        } else if (std::regex_match(line, match, bot_regex)) {
            int bot_id = std::stoi(match[1]);
            int low_target_type = (match[2] == "bot") ? 0 : 1; // 0 for bot, 1 for output
            int low_target_id = std::stoi(match[3]);
            int high_target_type = (match[4] == "bot") ? 0 : 1; // 0 for bot, 1 for output
            int high_target_id = std::stoi(match[5]);

            bot_instructions[bot_id] = {low_target_type * 1000 + low_target_id, high_target_type * 1000 + high_target_id}; // Encode target type and ID
        }
    }

    while (true) {
        bool changed = false;
        for (auto& [bot_id, chips] : bots) {
            if (chips.size() == 2) {
                std::sort(chips.begin(), chips.end());
                int low_chip = chips[0];
                int high_chip = chips[1];

                if (low_chip == 17 && high_chip == 61) {
                    std::cout << bot_id << std::endl;
                    return 0;
                }

                auto& instruction = bot_instructions[bot_id];
                int low_target_type = instruction.first / 1000;
                int low_target_id = instruction.first % 1000;
                int high_target_type = instruction.second / 1000;
                int high_target_id = instruction.second % 1000;

                if (low_target_type == 0) { // bot
                    bots[low_target_id].push_back(low_chip);
                } else { // output
                    outputs[low_target_id] = low_chip;
                }

                if (high_target_type == 0) { // bot
                    bots[high_target_id].push_back(high_chip);
                } else { // output
                    outputs[high_target_id] = high_chip;
                }

                chips.clear();
                changed = true;
            }
        }
        if (!changed) break;
    }

    return 0;
}
