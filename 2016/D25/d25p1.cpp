#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <limits>

enum Op { CPY, INC, DEC, JNZ, OUT };

struct Instr {
    Op  op;
    bool xReg; long long x;
    bool yReg; long long y;
};

inline long long val(const long long r[4], bool isReg, long long v)
{
    return isReg ? r[v] : v;
}

/* ------------------------------------------------------------
   Run the program once with a = 0 and return the value copied
   into register a at the *first* “cpy ? a” instruction.
   For AoC 2016‑25 this is exactly the bootstrap constant k.
   ------------------------------------------------------------ */
long long bootstrap(const std::vector<Instr>& prog)
{
    long long r[4] = {0,0,0,0};              // a,b,c,d
    std::size_t ip = 0;
    const std::size_t MAX = 500'000;         // ample safety margin

    for (std::size_t steps = 0; ip < prog.size() && steps < MAX; ++steps)
    {
        const Instr& ins = prog[ip];
        switch (ins.op)
        {
            case CPY:
                if (ins.yReg)
                {
                    const long long v = val(r, ins.xReg, ins.x);
                    if (ins.y == 0)          // first write to register a
                        return v;            // <-- k found
                    r[ins.y] = v;
                }
                ++ip;
                break;

            case INC: ++r[ins.x]; ++ip; break;
            case DEC: --r[ins.x]; ++ip; break;

            case JNZ:
                ip += (val(r, ins.xReg, ins.x) != 0)
                      ? val(r, ins.yReg, ins.y)
                      : 1;
                break;

            case OUT: ++ip; break;           // irrelevant for k
        }
    }
    return 0;                                // never reached with valid input
}

int main()
{
    /* --- read the assembunny program ----------------------------------- */
    std::vector<Instr> prog;
    std::string op,sx,sy;

    auto parse = [](const std::string& tok, bool& isReg, long long& v)
    {
        if (tok.size()==1 && std::islower(tok[0])) { isReg=true;  v=tok[0]-'a'; }
        else                                        { isReg=false; v=std::stoll(tok); }
    };

    while (std::cin >> op >> sx)
    {
        Instr in{};
        parse(sx, in.xReg, in.x);

        if (op == "cpy" || op == "jnz") { std::cin >> sy; parse(sy, in.yReg, in.y); }

        if      (op=="cpy") in.op = CPY;
        else if (op=="inc") in.op = INC;
        else if (op=="dec") in.op = DEC;
        else if (op=="jnz") in.op = JNZ;
        else                in.op = OUT;

        prog.push_back(in);
    }

    /* --- step 1: constant offset k -------------------------------------- */
    const long long k = bootstrap(prog);

    /* --- step 2: smallest “0101…” pattern above k ----------------------- */
    unsigned long long pattern = 2;          // binary 10 (LSB 0, next 1)

    while (pattern <= static_cast<unsigned long long>(k))
    {
        if (pattern > (std::numeric_limits<unsigned long long>::max() >> 2))
            return 1;                        // overflow safeguard
        pattern = pattern * 4 + 2;           // append another “10”
    }

    /* --- answer --------------------------------------------------------- */
    std::cout << (pattern - k) << std::endl;
    return 0;
}
