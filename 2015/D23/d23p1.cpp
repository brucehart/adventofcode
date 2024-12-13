#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <unordered_map>

// Enum representing the type of instruction
enum class OpCode {
    HLF,
    TPL,
    INC,
    JMP,
    JIE,
    JIO
};

// Struct representing a single instruction
struct Instruction {
    OpCode opcode;
    std::string registerName; // For operations that involve a register
    int offset;               // For jump operations
};

// Function to trim leading and trailing whitespace from a string
std::string trim(const std::string& s) {
    const auto start = s.find_first_not_of(" \t\r\n");
    const auto end = s.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

int main() {
    std::vector<Instruction> instructions;
    std::string line;

    // Read and parse each line from standard input
    while (std::getline(std::cin, line)) {
        std::istringstream iss(trim(line));
        std::string op;
        iss >> op;

        Instruction instr;
        instr.offset = 0; // Default offset

        if (op == "hlf") {
            instr.opcode = OpCode::HLF;
            iss >> instr.registerName;
        }
        else if (op == "tpl") {
            instr.opcode = OpCode::TPL;
            iss >> instr.registerName;
        }
        else if (op == "inc") {
            instr.opcode = OpCode::INC;
            iss >> instr.registerName;
        }
        else if (op == "jmp") {
            instr.opcode = OpCode::JMP;
            std::string offsetStr;
            iss >> offsetStr;
            instr.offset = std::stoi(offsetStr);
        }
        else if (op == "jie") {
            instr.opcode = OpCode::JIE;
            std::string regWithComma;
            iss >> regWithComma; // Read register with comma, e.g., "a,"
            // Remove the trailing comma
            if (!regWithComma.empty() && regWithComma.back() == ',') {
                regWithComma.pop_back();
            }
            instr.registerName = regWithComma;
            std::string offsetStr;
            iss >> offsetStr;
            instr.offset = std::stoi(offsetStr);
        }
        else if (op == "jio") {
            instr.opcode = OpCode::JIO;
            std::string regWithComma;
            iss >> regWithComma; // Read register with comma, e.g., "a,"
            // Remove the trailing comma
            if (!regWithComma.empty() && regWithComma.back() == ',') {
                regWithComma.pop_back();
            }
            instr.registerName = regWithComma;
            std::string offsetStr;
            iss >> offsetStr;
            instr.offset = std::stoi(offsetStr);
        }
        else {
            // Invalid operation; handle errors appropriately
            std::cerr << "Unknown operation: " << op << std::endl;
            return 1;
        }

        instructions.push_back(instr);
    }

    // Initialize registers 'a' and 'b' to 0
    std::unordered_map<std::string, long long> registers = { {"a", 0}, {"b", 0} };

    // Instruction pointer starts at 0
    long long ip = 0;

    // Execute instructions until the instruction pointer goes out of bounds
    while (ip >= 0 && ip < static_cast<long long>(instructions.size())) {
        const Instruction& current = instructions[ip];
        switch (current.opcode) {
            case OpCode::HLF:
                registers[current.registerName] /= 2;
                ip += 1;
                break;
            case OpCode::TPL:
                registers[current.registerName] *= 3;
                ip += 1;
                break;
            case OpCode::INC:
                registers[current.registerName] += 1;
                ip += 1;
                break;
            case OpCode::JMP:
                ip += current.offset;
                break;
            case OpCode::JIE:
                if (registers[current.registerName] % 2 == 0) {
                    ip += current.offset;
                }
                else {
                    ip += 1;
                }
                break;
            case OpCode::JIO:
                if (registers[current.registerName] == 1) {
                    ip += current.offset;
                }
                else {
                    ip += 1;
                }
                break;
            default:
                // Should not reach here
                std::cerr << "Invalid opcode encountered." << std::endl;
                return 1;
        }
    }

    // Output the value of register 'b'
    std::cout << registers["b"] << std::endl;

    return 0;
}

