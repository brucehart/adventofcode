#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <cstdint>
#include <algorithm>
#include <vector>

// We are dealing with a set of instructions that define signals on wires.
// Each instruction defines how to produce a signal for a single output wire.
// The signal might come directly from a constant, or from another wire,
// possibly passing through operations: AND, OR, NOT, LSHIFT, RSHIFT.
//
// Example instructions:
// "123 -> x"
// "x AND y -> z"
// "p LSHIFT 2 -> q"
// "NOT e -> f"
//
// We need to determine the final signal on wire "a".
//
// Approach:
// 1. Parse each line to extract the input wires/values, operation, and output wire.
// 2. Store these instructions in a structure keyed by the output wire's name.
// 3. To find the value on wire "a", recursively evaluate the instruction that produces "a".
//    - If it's a constant, return it.
//    - If it's a wire, recursively compute that wire's value.
//    - If it's an operation, compute the operation result from its inputs.
// 4. Use memoization to avoid re-computing wire values.
//
// After this, print the resulting value for wire "a".

enum class Operation {
    NONE,    // direct assignment (e.g. "123 -> x" or "x -> y")
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

// Try to convert a string to a number. If fails, it's a wire.
inline bool is_number(const std::string &s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

// Evaluate a wire or numeric value
uint16_t evaluate(const std::string &wire) {
    // If already computed, return cached value
    if (memo.find(wire) != memo.end()) {
        return memo[wire];
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

    // Read input lines until EOF
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }

        // Example patterns:
        // 1) "123 -> x"
        // 2) "x AND y -> z"
        // 3) "x OR y -> z"
        // 4) "x LSHIFT 2 -> f"
        // 5) "x RSHIFT 3 -> g"
        // 6) "NOT e -> f"
        // 7) "x -> y"

        std::stringstream ss(line);
        std::string part1, part2, part3, part4, arrow, outwire;

        // The structure of lines varies, but all end with "-> <wire>"
        // We'll parse greedily and use conditions to detect the pattern.

        // read until we get "->"
        // We know the last two tokens should be "-> outwire".
        // We'll parse line parts and then see what we got.

        // Possible forms:
        // - SINGLE: "<val> -> <wire>"
        // - NOT: "NOT <val> -> <wire>"
        // - BINARY: "<val> <OP> <val> -> <wire>"

        // Try reading tokens
        // The maximum tokens before "->" could be 3 (like "x AND y"), or 1 (like "123"),
        // or 2 (like "NOT x").
        // After reading tokens, we get arrow "->" and then outwire.

        // We'll parse from start:
        ss >> part1;
        ss >> part2;
        if (part2 == "->") {
            // form: "<val> -> <wire>"
            outwire = "";
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
                outwire = "";
                ss >> outwire;
                Instruction inst;
                inst.op = Operation::NOT;
                inst.lhs = part2;
                instructions[outwire] = inst;
            } else {
                // form: "<val1> <OP> <val2> -> <wire>"
                std::string op = part2;
                std::string val2 = part3;
                ss >> part4; // this should be "->"
                outwire = "";
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

    // Evaluate wire "a"
    std::cout << evaluate("a") << "\n";

    return 0;
}
