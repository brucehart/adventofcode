#include <iostream>
#include <string>

int main() {
    std::string batteries;
    long long total_joltage = 0;
    const int joltage_digits = 12;

    while (std::cin >> batteries) {
        // Greedily keep the lexicographically largest subsequence of length joltage_digits.
        const int digits_to_drop = static_cast<int>(batteries.size()) - joltage_digits;
        int remaining_drops = digits_to_drop > 0 ? digits_to_drop : 0;

        std::string chosen;
        chosen.reserve(batteries.size());

        for (char digit : batteries) {
            while (!chosen.empty() && remaining_drops > 0 && chosen.back() < digit) {
                chosen.pop_back();
                --remaining_drops;
            }
            chosen.push_back(digit);
        }

        if (remaining_drops > 0) {
            chosen.resize(chosen.size() - remaining_drops);
        }
        if (static_cast<int>(chosen.size()) > joltage_digits) {
            chosen.resize(joltage_digits);
        }

        long long bank_joltage = 0;
        for (int i = 0; i < joltage_digits && i < static_cast<int>(chosen.size()); ++i) {
            bank_joltage = bank_joltage * 10 + (chosen[i] - '0');
        }

        total_joltage += bank_joltage;
    }

    std::cout << total_joltage << std::endl;
}
