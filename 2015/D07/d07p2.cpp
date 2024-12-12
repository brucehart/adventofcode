#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <cstdint>
#include <algorithm>
#include <vector>

enum class Operation {
    NONE,    // direct assignment
    AND,
    OR,
    LSHIFT,
    RSHIFT,
    NOT
};

struct Instruction {
    Operation op = Operation::NONE;
    std::string lhs; // left-hand operand (wire or constant)
    std::string rhs; // right-hand operand (wire or constant), used only for binary ops
};

static std::unordered_map<std::string, Instruction> instructions;
static std::unordered_map<std::string, uint16_t> memo;

inline bool is_number(const std::string &s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

// Evaluate a wire or numeric value
uint16_t evaluate(const std::string &wire) {
    // If already computed, return cached value
    if (auto it = memo.find(wire); it != memo.end()) {
        return it->second;
    }

    uint16_t result = 0;

    // If wire is actually a numeric value (constant)
    if (is_number(wire)) {
        result = static_cast<uint16_t>(std::stoi(wire));
    } else {
        // Must be a wire defined by an instruction
        const auto &inst = instructions[wire];
        switch (inst.op) {
            case Operation::NONE:
                // Direct assignment
                result = evaluate(inst.lhs);
                break;
            case Operation::NOT:
                result = static_cast<uint16_t>(~evaluate(inst.lhs));
                break;
            case Operation::AND:
                result = static_cast<uint16_t>(evaluate(inst.lhs) & evaluate(inst.rhs));
                break;
            case Operation::OR:
                result = static_cast<uint16_t>(evaluate(inst.lhs) | evaluate(inst.rhs));
                break;
            case Operation::LSHIFT:
                result = static_cast<uint16_t>(evaluate(inst.lhs) << std::stoi(inst.rhs));
                break;
            case Operation::RSHIFT:
                result = static_cast<uint16_t>(evaluate(inst.lhs) >> std::stoi(inst.rhs));
                break;
        }
    }

    memo[wire] = result;
    return result;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Read input lines until EOF and store them
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);
        std::string part1, part2, part3, part4, outwire;

        ss >> part1;
        ss >> part2;
        if (part2 == "->") {
            // form: "<val> -> <wire>"
            ss >> outwire;
            Instruction inst;
            inst.op = Operation::NONE;
            inst.lhs = part1;
            instructions[outwire] = inst;
        } else {
            // could be NOT or a binary operation
            ss >> part3;
            if (part3 == "->") {
                // form: "NOT <val> -> <wire>"
                ss >> outwire;
                Instruction inst;
                inst.op = Operation::NOT;
                inst.lhs = part2;
                instructions[outwire] = inst;
            } else {
                // form: "<val1> <OP> <val2> -> <wire>"
                std::string op = part2;
                std::string val2 = part3;
                ss >> part4; // "->"
                ss >> outwire;

                Instruction inst;
                if (op == "AND") {
                    inst.op = Operation::AND;
                } else if (op == "OR") {
                    inst.op = Operation::OR;
                } else if (op == "LSHIFT") {
                    inst.op = Operation::LSHIFT;
                } else if (op == "RSHIFT") {
                    inst.op = Operation::RSHIFT;
                }
                inst.lhs = part1;
                inst.rhs = val2;
                instructions[outwire] = inst;
            }
        }
    }

    // Part 1: Evaluate wire "a"
    uint16_t a_value = evaluate("a");

    // Part 2: Override wire "b" with the value from part 1,
    // then reset the memo and re-evaluate "a".
    instructions["b"] = {Operation::NONE, std::to_string(a_value), ""};
    memo.clear(); // reset memoization

    std::cout << evaluate("a") << "\n";

    return 0;
}

