#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

// Structure to represent a dig instruction
struct Instruction {
    char direction;
    long long distance;
    std::string color;
};

// Function to parse the input and return a vector of instructions
std::vector<Instruction> parseInput() {
    std::vector<Instruction> instructions;
    char direction;
    int distance;
    std::string color;
    while (std::cin >> direction >> distance >> color) {
        instructions.push_back({direction, distance, color});
    }
    return instructions;
}

// Function to convert hexadecimal color code to instruction
Instruction decodeInstruction(const std::string& hex_code) {
    std::string distance_hex = hex_code.substr(2, 5);
    std::string direction_hex = hex_code.substr(7, 1);

    long long distance;
    std::stringstream ss;
    ss << std::hex << distance_hex;
    ss >> distance;

    char direction;
    if (direction_hex == "0") {
        direction = 'R';
    } else if (direction_hex == "1") {
        direction = 'D';
    } else if (direction_hex == "2") {
        direction = 'L';
    } else {
        direction = 'U';
    }

    return {direction, distance, ""}; 
}

int main() {
    // Parse the input instructions
    std::vector<Instruction> instructions = parseInput();

    // Decode the instructions from the hexadecimal codes
    std::vector<Instruction> decoded_instructions;
    for (const auto& instruction : instructions) {
        decoded_instructions.push_back(decodeInstruction(instruction.color));
    }

    // Calculate the vertices of the polygon formed by the dig plan
    std::vector<std::pair<long long, long long>> vertices;
    long long current_x = 0;
    long long current_y = 0;
    vertices.push_back({current_x, current_y});
    long long perimeter = 0;

    for (const auto& instruction : decoded_instructions) {
        perimeter += instruction.distance;
        if (instruction.direction == 'R') {
            current_x += instruction.distance;
        } else if (instruction.direction == 'L') {
            current_x -= instruction.distance;
        } else if (instruction.direction == 'D') {
            current_y += instruction.distance;
        } else if (instruction.direction == 'U') {
            current_y -= instruction.distance;
        }
        vertices.push_back({current_x, current_y});
    }

    // Calculate the area of the polygon using the Shoelace formula
    long long area = 0;
    for (size_t i = 0; i < vertices.size() - 1; ++i) {
        area += (vertices[i].first * vertices[i+1].second - vertices[i+1].first * vertices[i].second);
    }
    area = std::abs(area) / 2;

    // Calculate the number of interior points using Pick's theorem
    long long total_cubic_meters = area + perimeter / 2 + 1;

    std::cout << total_cubic_meters << std::endl;

    return 0;
}
