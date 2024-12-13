#include <iostream>
#include <string>
#include <regex>
#include <cstdint>

/**
 * Computes (base^exp) % mod using binary exponentiation.
 *
 * @param base The base value.
 * @param exp  The exponent.
 * @param mod  The modulus.
 * @return The result of (base^exp) % mod.
 */
uint64_t pow_mod(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base %= mod;
    while (exp > 0) {
        // If the least significant bit is set, multiply the result by the current base
        if (exp & 1)
            result = (result * base) % mod;
        // Square the base for the next bit
        base = (base * base) % mod;
        // Shift right to process the next bit
        exp >>= 1;
    }
    return result;
}

int main() {
    std::string input_line;
    
    // Read the entire input line from standard input
    std::getline(std::cin, input_line);
    
    // Define a regular expression to extract row and column numbers
    // The pattern looks for "row <number>, column <number>"
    std::regex pattern(R"(\brow\s+(\d+),\s+column\s+(\d+)\b)", std::regex_constants::icase);
    std::smatch matches;
    
    uint64_t row = 0, col = 0;
    
    if (std::regex_search(input_line, matches, pattern)) {
        if (matches.size() == 3) { // matches[0] is the full match, matches[1] and matches[2] are the captured groups
            row = std::stoull(matches[1].str());
            col = std::stoull(matches[2].str());
        } else {
            std::cerr << "Error: Unable to parse row and column numbers." << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Error: Input does not contain valid row and column information." << std::endl;
        return 1;
    }
    
    // Calculate the diagonal number
    // Diagonal numbering starts at 1 for (1,1), 2 for (2,1) and (1,2), etc.
    uint64_t diagonal = row + col - 1;
    
    // Calculate the position index using the triangle number formula
    // Index = T(diagonal - 1) + column, where T(n) = n*(n+1)/2
    uint64_t index = (diagonal * (diagonal - 1)) / 2 + col;
    
    // Constants as per the problem statement
    const uint64_t initial_code = 20151125;
    const uint64_t multiplier = 252533;
    const uint64_t modulus = 33554393;
    
    // Compute the code at the given position using modular exponentiation
    // code = (initial_code * (multiplier^(index-1) mod modulus)) mod modulus
    uint64_t multiplier_power = pow_mod(multiplier, index - 1, modulus);
    uint64_t code = (initial_code * multiplier_power) % modulus;
    
    // Output the resulting code
    std::cout << code << std::endl;
    
    return 0;
}
