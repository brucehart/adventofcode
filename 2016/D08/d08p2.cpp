#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>

int main() {
    constexpr int width = 50;
    constexpr int height = 6;

    std::vector<std::vector<bool>> screen(height, std::vector<bool>(width, false));

    std::string line;
    std::regex rect_regex("rect (\\d+)x(\\d+)");
    std::regex row_regex("rotate row y=(\\d+) by (\\d+)");
    std::regex col_regex("rotate column x=(\\d+) by (\\d+)");

    while (std::getline(std::cin, line)) {
        std::smatch match;

        if (std::regex_match(line, match, rect_regex)) {
            int a = std::stoi(match[1]);
            int b = std::stoi(match[2]);
            for (int y = 0; y < b; ++y) {
                for (int x = 0; x < a; ++x) {
                    screen[y][x] = true;
                }
            }
        } else if (std::regex_match(line, match, row_regex)) {
            int y = std::stoi(match[1]);
            int b = std::stoi(match[2]);
            std::vector<bool> temp(width);
            for (int x = 0; x < width; ++x) {
                temp[(x + b) % width] = screen[y][x];
            }
            screen[y] = temp;
        } else if (std::regex_match(line, match, col_regex)) {
            int x = std::stoi(match[1]);
            int b = std::stoi(match[2]);
            std::vector<bool> temp(height);
            for (int y = 0; y < height; ++y) {
                temp[(y + b) % height] = screen[y][x];
            }
            for (int y = 0; y < height; ++y) {
                screen[y][x] = temp[y];
            }
        }
    }

    // Print the screen
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            std::cout << (screen[y][x] ? '#' : '.');
        }
        std::cout << std::endl;
    }


    int lit_pixels = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (screen[y][x]) {
                lit_pixels++;
            }
        }
    }

    //std::cout << lit_pixels << std::endl;


    return 0;
}