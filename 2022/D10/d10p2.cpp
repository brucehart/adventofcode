#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <array>

// Struct to represent each instruction
struct Instruction {
    std::string op;
    int value; // Only used for 'addx' instructions
};

int main() {
    // Read all instructions from standard input
    std::vector<Instruction> instructions;
    std::string line;
    while (std::getline(std::cin, line)) {
        std::stringstream ss(line);
        Instruction instr;
        ss >> instr.op;
        if (instr.op == "addx") {
            ss >> instr.value;
        } else {
            instr.value = 0; // 'noop' has no associated value
        }
        instructions.push_back(instr);
    }

    // Initialize CRT screen: 6 rows of 40 pixels each
    const int screen_width = 40;
    const int screen_height = 6;
    std::vector<std::string> screen(screen_height, std::string(screen_width, '.'));

    int X = 1; // Register X starts at 1
    int cycle = 1; // Current cycle number
    size_t current_instruction = 0; // Index of the current instruction
    int remaining_cycles = 0; // Remaining cycles for the current instruction
    int pending_value = 0; // Value to add to X after 'addx' completes

    // Total cycles to simulate (40 pixels * 6 rows)
    const int total_cycles = screen_width * screen_height;

    for (; cycle <= total_cycles; ++cycle) {
        // Determine current pixel position
        int pixel_position = (cycle - 1) % screen_width;
        int current_row = (cycle - 1) / screen_width;

        // Check if the current pixel is within the sprite's position
        if (pixel_position >= (X - 1) && pixel_position <= (X + 1)) {
            screen[current_row][pixel_position] = '#';
        } else {
            screen[current_row][pixel_position] = '.';
        }

        // If no instruction is currently being processed, fetch the next one
        if (remaining_cycles == 0 && current_instruction < instructions.size()) {
            const Instruction& instr = instructions[current_instruction];
            if (instr.op == "noop") {
                remaining_cycles = 1;
                pending_value = 0; // 'noop' has no effect
            } else if (instr.op == "addx") {
                remaining_cycles = 2;
                pending_value = instr.value; // Store the value to add after execution
            }
            current_instruction++;
        }

        // Decrement the remaining cycles for the current instruction
        if (remaining_cycles > 0) {
            remaining_cycles--;
            // If the instruction has just completed, apply its effect
            if (remaining_cycles == 0 && pending_value != 0) {
                X += pending_value;
                pending_value = 0; // Reset after applying
            }
        }
    }

    // Output the CRT screen
    for (const auto& row : screen) {
        std::cout << row << std::endl;
    }

    // Optionally, you can add an std::endl at the end as per the instructions
    std::cout << std::endl;

    return 0;
}

