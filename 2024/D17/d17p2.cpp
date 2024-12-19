#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>
#include <cmath>

/**
 * @brief Represents the state of the machine with registers, instruction pointer,
 * program input, and output.
 */
struct State {
    uint64_t A; // Register A
    uint64_t B; // Register B
    uint64_t C; // Register C
    uint64_t ip; // Instruction Pointer
    std::vector<uint8_t> prog; // Program Input
    std::vector<uint8_t> out;  // Program Output

    State() : A(0), B(0), C(0), ip(0), prog(), out() {}
};

// Global state variables
State Chrono;
State OrgChrono;

/**
 * @brief Parses the input from standard input to initialize OrgChrono.
 * 
 * The expected input format is:
 * Register A: <number>
 * Register B: <number>
 * Register C: <number>
 * Program: <comma-separated digits>
 */
void parseInput() {
    std::string line;

    // Parse Register A
    if (std::getline(std::cin, line)) {
        std::sscanf(line.c_str(), "Register A: %llu", (unsigned long long*)&OrgChrono.A);
    }

    // Parse Register B
    if (std::getline(std::cin, line)) {
        std::sscanf(line.c_str(), "Register B: %llu", (unsigned long long*)&OrgChrono.B);
    }

    // Parse Register C
    if (std::getline(std::cin, line)) {
        std::sscanf(line.c_str(), "Register C: %llu", (unsigned long long*)&OrgChrono.C);
    }

    // Skip possible empty lines until "Program:" line
    while (std::getline(std::cin, line)) {
        if (line.find("Program:") != std::string::npos) {
            break;
        }
    }

    // Parse Program
    size_t pos = line.find(':');
    if (pos != std::string::npos) {
        std::string progStr = line.substr(pos + 1);
        // Remove all whitespace characters
        progStr.erase(std::remove_if(progStr.begin(), progStr.end(), ::isspace), progStr.end());
        // Parse digits, ignoring commas
        OrgChrono.prog.clear();
        for (char c : progStr) {
            if (c == ',') continue;
            OrgChrono.prog.push_back(static_cast<uint8_t>(c - '0'));
        }
    }
}

/**
 * @brief Maps the operation code based on the current state.
 * 
 * If op > 3, it maps specific operation codes to register values.
 * Otherwise, it returns the operation code unchanged.
 * 
 * @param op Operation code to map.
 * @return Mapped operation value.
 */
uint64_t comboOp(uint64_t op) {
    if (op > 3) {
        switch (op) {
            case 4:
                return Chrono.A;
            case 5:
                return Chrono.B;
            case 6:
                return Chrono.C;
            default:
                return op;
        }
    }
    return op;
}

/**
 * @brief Performs the 'adv' operation: A = A / 2^op
 * 
 * @param op Operation code.
 */
void adv(uint64_t op) {
    op = comboOp(op);
    if (op >= 64) {
        Chrono.A = 0; // Prevent undefined behavior for large shifts
    } else {
        Chrono.A /= (1ULL << op);
    }
}

/**
 * @brief Performs the 'bdv' operation: B = A / 2^op
 * 
 * @param op Operation code.
 */
void bdv(uint64_t op) {
    op = comboOp(op);
    if (op >= 64) {
        Chrono.B = 0;
    } else {
        Chrono.B = Chrono.A / (1ULL << op);
    }
}

/**
 * @brief Performs the 'cdv' operation: C = A / 2^op
 * 
 * @param op Operation code.
 */
void cdv(uint64_t op) {
    op = comboOp(op);
    if (op >= 64) {
        Chrono.C = 0;
    } else {
        Chrono.C = Chrono.A / (1ULL << op);
    }
}

/**
 * @brief Performs the 'bxl' operation: B = B XOR op
 * 
 * @param op Operation code.
 */
void bxl(uint64_t op) {
    Chrono.B ^= op;
}

/**
 * @brief Performs the 'bst' operation: B = comboOp(op) % 8
 * 
 * @param op Operation code.
 */
void bst(uint64_t op) {
    Chrono.B = comboOp(op) % 8;
}

/**
 * @brief Performs the 'bxc' operation: B = B XOR C
 */
void bxc() {
    Chrono.B ^= Chrono.C;
}

/**
 * @brief Performs the 'jnz' operation: if A != 0, set ip = op; else, ip += 2
 * 
 * @param op Operation code.
 */
void jnz(uint64_t op) {
    if (Chrono.A != 0) {
        Chrono.ip = op;
    } else {
        Chrono.ip += 2;
    }
}

/**
 * @brief Performs the 'out' operation: append (comboOp(op) % 8) to output
 * 
 * @param op Operation code.
 */
void out_op(uint64_t op) {
    op = comboOp(op) % 8;
    Chrono.out.push_back(static_cast<uint8_t>(op));
}

/**
 * @brief Executes the program with the given initial value for register A.
 * 
 * @param init Initial value for register A.
 * @return The output of the program as a string.
 */
std::string run(uint64_t init) {
    // Initialize Chrono with OrgChrono and set A to init
    Chrono = OrgChrono;
    Chrono.A = init;
    Chrono.ip = 0;
    Chrono.out.clear();

    // Execute the program
    while (Chrono.ip + 1 < Chrono.prog.size()) {
        uint64_t opr = static_cast<uint64_t>(Chrono.prog[Chrono.ip + 1]);
        uint8_t opcode = Chrono.prog[Chrono.ip];
        switch (opcode) {
            case 0:
                adv(opr);
                Chrono.ip += 2;
                break;
            case 1:
                bxl(opr);
                Chrono.ip += 2;
                break;
            case 2:
                bst(opr);
                Chrono.ip += 2;
                break;
            case 3:
                jnz(opr);
                // If jump occurred, continue without incrementing ip
                if (Chrono.A != 0) {
                    continue;
                }
                break;
            case 4:
                bxc();
                Chrono.ip += 2;
                break;
            case 5:
                out_op(opr);
                Chrono.ip += 2;
                break;
            case 6:
                bdv(opr);
                Chrono.ip += 2;
                break;
            case 7:
                cdv(opr);
                Chrono.ip += 2;
                break;
            default:
                // Unknown opcode, terminate execution
                return "";
        }
    }

    // Convert output bytes to string by adding '0' to each byte
    std::string output;
    output.reserve(Chrono.out.size());
    for (auto byte : Chrono.out) {
        output += static_cast<char>(byte + '0');
    }
    return output;
}

/**
 * @brief Recursively searches for the initial value of A that causes the program's
 * output to match the program itself.
 * 
 * @param a Current value of A being tested.
 * @param i Current recursion depth.
 * @return The valid value of A if found; otherwise, 0.
 */
uint64_t solve(uint64_t a, int i) {
    std::string out = run(a);

    // Convert OrgChrono.prog to a string for comparison
    std::string progStr;
    progStr.reserve(OrgChrono.prog.size());
    for (auto c : OrgChrono.prog) {
        progStr += static_cast<char>(c + '0');
    }

    // Check if the output matches the program
    if (out == progStr) {
        return a;
    }

    // If it's the initial call or the last i characters match, continue searching
    if (i == 0 || (out.size() >= static_cast<size_t>(i) && 
        out.substr(out.size() - i) == progStr.substr(progStr.size() - i))) {
        for (int ni = 0; ni < 8; ++ni) { // Try all digits 0-7
            uint64_t na = solve(8 * a + ni, i + 1);
            if (na > 0) {
                return na;
            }
        }
    }

    // No valid A found in this path
    return 0;
}

/**
 * @brief Converts a byte array to a string by adding '0' to each byte.
 * 
 * @param bytes The byte array to convert.
 * @return The resulting string.
 */
std::string byteArrStr(const std::vector<uint8_t>& bytes) {
    std::string s;
    s.reserve(bytes.size());
    for (auto b : bytes) {
        s += static_cast<char>(b + '0');
    }
    return s;
}

/**
 * @brief The main function that orchestrates parsing input, solving for A, and
 * validating the solution.
 * 
 * It reads the input, attempts to find the correct initial value for register A,
 * runs the program with this value, and outputs the result.
 * If the solution is invalid, it reports a failure.
 * 
 * @return Exit status code.
 */
int main() {
    // Parse input from stdin
    parseInput();

    // Convert OrgChrono.prog to string for comparison
    std::string progStr;
    progStr.reserve(OrgChrono.prog.size());
    for (auto c : OrgChrono.prog) {
        progStr += static_cast<char>(c + '0');
    }

    // Attempt to find the correct initial value for register A
    uint64_t A = solve(0, 0);

    // If no solution is found, report and exit
    if (A == 0) {
        std::cerr << "No solution found" << std::endl;
        return 1;
    }

    // Run the program with the found value of A
    std::string out = run(A);

    // Validate the output matches the program
    if (out != progStr) {
        std::cerr << "Failed Test\n\tGot " << out << ", Want " << progStr << std::endl;
        return 1;
    }

    // Output the valid value of A
    std::cout << A << std::endl;
    return 0;
}
