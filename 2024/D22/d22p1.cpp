#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

// Function to generate the next secret number
uint32_t next_secret(uint32_t secret) {
    // Step 1: Multiply by 64, XOR, and prune
    uint32_t step1 = secret * 64;
    secret ^= step1;
    secret %= 16777216;

    // Step 2: Divide by 32 (floor), XOR, and prune
    uint32_t step2 = secret / 32;
    secret ^= step2;
    secret %= 16777216;

    // Step 3: Multiply by 2048, XOR, and prune
    uint32_t step3 = secret * 2048;
    secret ^= step3;
    secret %= 16777216;

    return secret;
}

int main() {
    std::vector<uint32_t> secrets;
    std::string line;

    // Read all initial secret numbers from stdin
    while (std::getline(std::cin, line)) {
        if (!line.empty()) {
            secrets.emplace_back(static_cast<uint32_t>(std::stoul(line)));
        }
    }

    uint64_t total = 0;

    // For each secret, simulate 2000 iterations
    for (auto &secret : secrets) {
        uint32_t current = secret;
        for (int i = 0; i < 2000; ++i) {
            current = next_secret(current);
        }
        total += current;
    }

    // Output the sum of the 2000th secret numbers
    std::cout << total << std::endl;
}

