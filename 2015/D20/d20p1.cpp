#include <iostream>
#include <vector>
#include <cstdint>

int main() {
    // Read the target number of presents from standard input
    uint32_t target;
    std::cin >> target;

    // Estimate an upper bound for the house number.
    // This can be adjusted if the target is not met within this range.
    const uint32_t MAX_HOUSE = 10000000;

    // Initialize a vector to store the total presents for each house.
    // Index 0 is unused to make house numbering start at 1.
    std::vector<uint32_t> presents(MAX_HOUSE + 1, 0);

    // Sieve-like approach to calculate the sum of presents for each house.
    // Each Elf (numbered 'elf') delivers presents to every multiple of 'elf'.
    for (uint32_t elf = 1; elf <= MAX_HOUSE; ++elf) {
        // Start delivering from the house numbered 'elf' and skip by 'elf' each time
        for (uint32_t house = elf; house <= MAX_HOUSE; house += elf) {
            presents[house] += elf * 10; // Each Elf delivers 10 times their number
        }
    }

    // Iterate through the houses to find the first house that meets or exceeds the target
    for (uint32_t house = 1; house <= MAX_HOUSE; ++house) {
        if (presents[house] >= target) {
            std::cout << house << std::endl;
            return 0; // Exit once the first qualifying house is found
        }
    }

    // If no house meets the target within the range, output a message.
    std::cout << "No house found within the range up to " << MAX_HOUSE << "." << std::endl;
    return 0;
}

