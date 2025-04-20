#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <array>
#include <cctype>

enum Op { CPY, INC, DEC, JNZ, TGL };

struct Arg {
    bool is_reg;
    int val;  // register index (0-3) if is_reg, else immediate value
};

struct Instr {
    Op op;
    Arg x, y;
    int argCount;  // 1 or 2
};

// Parse a token into an Arg
Arg parseArg(const std::string &tok) {
    if (std::isalpha(tok[0])) {
        return { true, tok[0] - 'a' };
    } else {
        return { false, std::stoi(tok) };
    }
}

// Get the value of an argument (register or immediate)
int getValue(const Arg &a, const std::array<int,4> &regs) {
    return a.is_reg ? regs[a.val] : a.val;
}

// Toggle the instruction according to the rules
void toggle(Instr &instr) {
    if (instr.argCount == 1) {
        // one-argument: inc -> dec, others -> inc
        instr.op = (instr.op == INC ? DEC : INC);
    } else if (instr.argCount == 2) {
        // two-argument: jnz <-> cpy
        instr.op = (instr.op == JNZ ? CPY : JNZ);
    }
}

int main() {
    // Read and parse program from stdin
    std::vector<Instr> program;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string opTok, a, b;
        iss >> opTok >> a;
        Instr instr;
        instr.x = parseArg(a);

        if (opTok == "cpy")       instr.op = CPY;
        else if (opTok == "inc")  instr.op = INC;
        else if (opTok == "dec")  instr.op = DEC;
        else if (opTok == "jnz")  instr.op = JNZ;
        else if (opTok == "tgl")  instr.op = TGL;
        else                      continue;  // skip unknown

        if (instr.op == CPY || instr.op == JNZ) {
            iss >> b;
            instr.y = parseArg(b);
            instr.argCount = 2;
        } else {
            // inc, dec, tgl are one-argument
            instr.y = { false, 0 };
            instr.argCount = 1;
        }

        program.push_back(instr);
    }

    // Registers a, b, c, d; initialize a = 7 (number of eggs)
    std::array<int,4> regs = {7, 0, 0, 0};

    // Execute the program
    for (int ip = 0; ip >= 0 && ip < static_cast<int>(program.size()); ) {
        Instr &instr = program[ip];
        switch (instr.op) {
            case CPY:
                // only copy if destination is a register
                if (instr.y.is_reg) {
                    regs[instr.y.val] = getValue(instr.x, regs);
                }
                ip++;
                break;

            case INC:
                if (instr.x.is_reg) {
                    regs[instr.x.val]++;
                }
                ip++;
                break;

            case DEC:
                if (instr.x.is_reg) {
                    regs[instr.x.val]--;
                }
                ip++;
                break;

            case JNZ:
                if (getValue(instr.x, regs) != 0) {
                    ip += getValue(instr.y, regs);
                } else {
                    ip++;
                }
                break;

            case TGL: {
                int target = ip + getValue(instr.x, regs);
                // toggle if within bounds
                if (target >= 0 && target < static_cast<int>(program.size())) {
                    toggle(program[target]);
                }
                ip++;
                break;
            }

            default:
                ip++;
                break;
        }
    }

    // Output the value in register a
    std::cout << regs[0] << std::endl;
    return 0;
}

