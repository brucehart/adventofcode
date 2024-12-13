#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

int main() {
    // Read the target number of presents from standard input
    uint64_t target;
    std::cin >> target;

    // Estimate an upper bound for the house number.
    // Each elf delivers to up to 50 houses, so the maximum house number
    // we need to consider is 50 times the target divided by the minimum presents per elf.
    // To ensure we cover all possibilities, we'll set a generous upper limit.
    const uint32_t MAX_HOUSE = 10000000; // 10 million

    // Initialize a vector to store the total presents for each house.
    // Index 0 is unused to make house numbering start at 1.
    // Using uint64_t to prevent potential overflow.
    std::vector<uint64_t> presents(MAX_HOUSE + 1, 0);

    // Sieve-like approach to calculate the sum of presents for each house.
    // Each Elf (numbered 'elf') delivers presents to every multiple of 'elf',
    // but only up to 50 houses.
    for (uint32_t elf = 1; elf <= MAX_HOUSE; ++elf) {
        // Calculate the last house this elf will visit
        uint32_t last_house = std::min(elf * 50, MAX_HOUSE);

        // Iterate through each house this elf will visit
        for (uint32_t house = elf; house <= last_house; house += elf) {
            presents[house] += static_cast<uint64_t>(elf) * 11; // Each Elf delivers 11 times their number
        }

        // Optional: Progress indicator for large computations
        // Uncomment the following lines to see progress every 1,000,000 elves
        /*
        if (elf % 1000000 == 0) {
            std::cerr << "Processed elf " << elf << std::endl;
        }
        */
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

