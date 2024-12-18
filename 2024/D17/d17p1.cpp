#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

// Function to get the value of a combo operand
unsigned long long getComboValue(int operand, unsigned long long regA, unsigned long long regB, unsigned long long regC) {
    if (operand >= 0 && operand <= 3) {
        return operand;
    } else if (operand == 4) {
        return regA;
    } else if (operand == 5) {
        return regB;
    } else if (operand == 6) {
        return regC;
    }
    return 0; // Should not happen in valid programs
}

int main() {
    unsigned long long regA, regB, regC;
    std::string line;

    // Read initial register values
    std::getline(std::cin, line);
    sscanf(line.c_str(), "Register A: %llu", &regA);
    std::getline(std::cin, line);
    sscanf(line.c_str(), "Register B: %llu", &regB);
    std::getline(std::cin, line);
    sscanf(line.c_str(), "Register C: %llu", &regC);

    // Read program
    std::getline(std::cin, line); // Skip "Program:" line
    std::getline(std::cin, line);
    if (line.find("Program: ") == 0) {
        line = line.substr(9);
    }
    std::vector<int> program;
    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    while ((pos = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, pos);        
        program.push_back(std::stoi(token));
        line.erase(0, pos + delimiter.length());
    }
    program.push_back(std::stoi(line));

    // Run the program
    int ip = 0;
    std::vector<int> output;
    while (ip >= 0 && ip < program.size()) {
        int opcode = program[ip];
        int operand = program[ip + 1];

        switch (opcode) {
            case 0: // adv
                regA = regA / static_cast<int>(std::pow(2, getComboValue(operand, regA, regB, regC)));
                ip += 2;
                break;
            case 1: // bxl
                regB = regB ^ operand;
                ip += 2;
                break;
            case 2: // bst
                regB = getComboValue(operand, regA, regB, regC) % 8;
                ip += 2;
                break;
            case 3: // jnz
                if (regA != 0) {
                    ip = operand;
                } else {
                    ip += 2;
                }
                break;
            case 4: // bxc
                regB = regB ^ regC;
                ip += 2;
                break;
            case 5: // out
                output.push_back(getComboValue(operand, regA, regB, regC) % 8);
                ip += 2;
                break;
            case 6: // bdv
                regB = regA / static_cast<int>(std::pow(2, getComboValue(operand, regA, regB, regC)));
                ip += 2;
                break;
            case 7: // cdv
                regC = regA / static_cast<int>(std::pow(2, getComboValue(operand, regA, regB, regC)));
                ip += 2;
                break;
        }
    }

    // Output the results
    for (size_t i = 0; i < output.size(); ++i) {
        std::cout << output[i];
        if (i < output.size() - 1) {
            std::cout << ",";
        }
    }
    std::cout << std::endl;

    return 0;
}