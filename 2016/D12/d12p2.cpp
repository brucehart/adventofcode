#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <array>

struct Instr {
    std::string op, x, y;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Read and parse all instructions
    std::vector<Instr> program;
    for (std::string line; std::getline(std::cin, line); ) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        Instr inst;
        iss >> inst.op >> inst.x;
        if (inst.op == "cpy" || inst.op == "jnz")
            iss >> inst.y;
        program.push_back(inst);
    }

    // Registers a=0, b=1, c=2, d=3; initialize c to 1 for part two
    std::array<int, 4> reg = {0, 0, 1, 0};

    auto is_register = [&](const std::string &s) {
        return s.size() == 1 && s[0] >= 'a' && s[0] <= 'd';
    };
    auto get_value = [&](const std::string &s) {
        if (is_register(s))
            return reg[s[0] - 'a'];
        return std::stoi(s);
    };

    // Execute the assembunny program
    for (int ip = 0; ip < static_cast<int>(program.size()); ) {
        const auto &in = program[ip];
        if (in.op == "cpy") {
            reg[in.y[0] - 'a'] = get_value(in.x);
            ++ip;
        }
        else if (in.op == "inc") {
            ++reg[in.x[0] - 'a'];
            ++ip;
        }
        else if (in.op == "dec") {
            --reg[in.x[0] - 'a'];
            ++ip;
        }
        else if (in.op == "jnz") {
            if (get_value(in.x) != 0)
                ip += get_value(in.y);
            else
                ++ip;
        }
        else {
            ++ip;  // skip unknown
        }
    }

    // Output the value in register a after execution
    std::cout << reg[0] << std::endl;
    return 0;
}

