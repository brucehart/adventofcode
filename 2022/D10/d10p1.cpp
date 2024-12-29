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

    // Define the cycles at which to record signal strengths
    const std::array<int, 6> target_cycles = {20, 60, 100, 140, 180, 220};
    size_t target_index = 0; // Index to track the current target cycle

    int X = 1; // Register X starts at 1
    int cycle = 1; // Current cycle number
    long long signal_sum = 0; // Sum of the signal strengths

    size_t current_instruction = 0; // Index of the current instruction
    int remaining_cycles = 0; // Remaining cycles for the current instruction
    int pending_value = 0; // Value to add to X after 'addx' completes

    // Continue processing until all target cycles are covered
    while (target_index < target_cycles.size()) {
        // Check if the current cycle is a target cycle
        if (cycle == target_cycles[target_index]) {
            signal_sum += static_cast<long long>(cycle) * X;
            target_index++;
        }

        // If no instruction is currently being processed, fetch the next one
        if (remaining_cycles == 0 && current_instruction < instructions.size()) {
            const Instruction& instr = instructions[current_instruction];
            if (instr.op == "noop") {
                remaining_cycles = 1;
                pending_value = 0; // No effect for 'noop'
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

        cycle++; // Move to the next cycle
    }

    // Output the sum of the signal strengths
    std::cout << signal_sum << std::endl;

    return 0;
}

