#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <array>
#include <cctype>

enum Op { CPY, INC, DEC, JNZ, TGL };

struct Arg {
    bool is_reg;
    int val;  // register index if is_reg, else immediate value
};

struct Instr {
    Op op;
    Arg x, y;
    int argCount;  // 1 or 2
};

// Parse a token into an Arg (register or immediate)
static Arg parseArg(const std::string &tok) {
    if (std::isalpha(tok[0])) {
        return { true, tok[0] - 'a' };
    } else {
        return { false, std::stoi(tok) };
    }
}

// Get the runtime value of an argument
static int getValue(const Arg &a, const std::array<int,4> &regs) {
    return a.is_reg ? regs[a.val] : a.val;
}

// Toggle an instruction in-place
static void toggle(Instr &instr) {
    if (instr.argCount == 1) {
        instr.op = (instr.op == INC ? DEC : INC);
    } else if (instr.argCount == 2) {
        instr.op = (instr.op == JNZ ? CPY : JNZ);
    }
}

int main() {
    // Read and parse the assembunny program from stdin
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
        else                      continue;

        if (instr.op == CPY || instr.op == JNZ) {
            iss >> b;
            instr.y = parseArg(b);
            instr.argCount = 2;
        } else {
            instr.y = { false, 0 };
            instr.argCount = 1;
        }
        program.push_back(instr);
    }

    // Registers a, b, c, d; initialize a = 12 for part two
    std::array<int,4> regs = {12, 0, 0, 0};

    // Execute with instruction pointer ip
    for (int ip = 0; ip >= 0 && ip < static_cast<int>(program.size()); ) {
        // Peephole: detect and optimize simple multiplication loops
        if (ip + 5 < static_cast<int>(program.size())) {
            Instr &i0 = program[ip];
            Instr &i1 = program[ip+1];
            Instr &i2 = program[ip+2];
            Instr &i3 = program[ip+3];
            Instr &i4 = program[ip+4];
            Instr &i5 = program[ip+5];

            // Pattern:
            //   cpy X C
            //   inc A
            //   dec C
            //   jnz C -2
            //   dec D
            //   jnz D -5
            if (i0.op == CPY && i1.op == INC && i2.op == DEC
                && i3.op == JNZ && i4.op == DEC && i5.op == JNZ
                && i3.x.is_reg && i3.x.val == i2.x.val
                && !i3.y.is_reg && i3.y.val == -2
                && i5.x.is_reg && i5.x.val == i4.x.val
                && !i5.y.is_reg && i5.y.val == -5
                && i0.y.is_reg && i1.x.is_reg && i2.x.is_reg && i4.x.is_reg)
            {
                int mulVal = getValue(i0.x, regs);
                int times  = regs[i4.x.val];
                // Perform A += X * D
                regs[i1.x.val] += mulVal * times;
                // Zero out C and D
                regs[i2.x.val] = 0;
                regs[i4.x.val] = 0;
                ip += 6;
                continue;
            }
        }

        Instr &instr = program[ip];
        switch (instr.op) {
            case CPY:
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

