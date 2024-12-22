#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>

// Function to generate the next secret number based on the current secret
uint32_t next_secret(uint32_t secret) {
    // Step 1: Multiply by 64, XOR, and prune
    uint32_t step1 = secret * 64;
    secret ^= step1;
    secret %= 16777216; // Prune to 24 bits

    // Step 2: Divide by 32 (floor), XOR, and prune
    uint32_t step2 = secret / 32;
    secret ^= step2;
    secret %= 16777216; // Prune to 24 bits

    // Step 3: Multiply by 2048, XOR, and prune
    uint32_t step3 = secret * 2048;
    secret ^= step3;
    secret %= 16777216; // Prune to 24 bits

    return secret;
}

int main() {
    std::vector<uint32_t> initial_secrets;
    std::string line;

    // Read all initial secret numbers from stdin
    while (std::getline(std::cin, line)) {
        if (!line.empty()) {
            initial_secrets.emplace_back(static_cast<uint32_t>(std::stoul(line)));
        }
    }

    // Define constants for sequence encoding
    const int BASE = 19;
    const int POW3 = BASE * BASE * BASE; // 19^3 = 6859
    const int SEQUENCE_COUNT = 130321; // 19^4

    // Initialize global arrays
    std::vector<uint64_t> sum_of_prices(SEQUENCE_COUNT, 0); // Sum of prices for each sequence
    std::vector<int> last_buyer_id(SEQUENCE_COUNT, 0); // Last buyer that processed each sequence

    int buyer_id = 1; // Unique identifier for each buyer

    // Iterate through each buyer
    for (const auto& initial_secret : initial_secrets) {
        // Simulate the sequence of secret numbers
        std::vector<uint32_t> secrets;
        secrets.reserve(2001);
        secrets.emplace_back(initial_secret);
        for (int i = 0; i < 2000; ++i) {
            secrets.emplace_back(next_secret(secrets.back()));
        }

        // Derive prices from secret numbers (last digit)
        std::vector<int> prices;
        prices.reserve(2001);
        for (const auto& secret : secrets) {
            prices.emplace_back(static_cast<int>(secret % 10));
        }

        // Compute changes between consecutive prices
        std::vector<int> changes;
        changes.reserve(2000);
        for (size_t i = 1; i < prices.size(); ++i) {
            changes.emplace_back(prices[i] - prices[i - 1]);
        }

        // Initialize variables for sliding window of four changes
        uint32_t key = 0;
        int count = 0;

        // Iterate through each change to form sequences of four consecutive changes
        for (size_t i = 0; i < changes.size(); ++i) {
            int current_change = changes[i];
            // Map change from [-9,9] to [0,18] by adding 9
            int mapped_change = current_change + 9;
            // Update the rolling key
            if (count < 4) {
                key = key * BASE + mapped_change;
                count++;
            } else {
                key = (key % POW3) * BASE + mapped_change;
            }

            // Once we have four changes, process the sequence
            if (count >= 4) {
                // Check if this buyer has already processed this sequence
                if (last_buyer_id[key] < buyer_id) {
                    // Add the current price to the sum for this sequence
                    sum_of_prices[key] += static_cast<uint64_t>(prices[i + 1]);
                    // Mark this sequence as processed by the current buyer
                    last_buyer_id[key] = buyer_id;
                }
            }
        }

        buyer_id++; // Increment buyer ID for the next buyer
    }

    // Find the maximum sum of bananas across all sequences
    uint64_t max_bananas = 0;
    for (const auto& total : sum_of_prices) {
        if (total > max_bananas) {
            max_bananas = total;
        }
    }

    // Output the maximum number of bananas
    std::cout << max_bananas << std::endl;

    return 0;
}

