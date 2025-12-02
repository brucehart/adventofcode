#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace {

int numDigits(long long value) {
    if (value == 0) return 1;
    int digits = 0;
    long long v = value;
    while (v) {
        v /= 10;
        ++digits;
    }
    return digits;
}

long long pow10(int exp) {
    long long result = 1;
    for (int i = 0; i < exp; ++i) result *= 10;
    return result;
}

// Sum the values within [low, high] whose upper and lower halves match (even digit counts only).
long long sumDoubleNumbers(long long low, long long high) {
    if (low > high) return 0;

    int digits = numDigits(low);
    int halfDigits = digits / 2;
    long long halfPower = pow10(halfDigits);

    auto makeNumber = [&](long long upper) {
        return upper * halfPower + upper;
    };

    long long upper = low / halfPower;
    while (makeNumber(upper) < low) {
        ++upper;
    }

    long long subtotal = 0;
    for (long long u = upper;; ++u) {
        long long candidate = makeNumber(u);
        if (candidate > high) break;
        subtotal += candidate;
    }

    return subtotal;
}

}  // namespace

int main() {
    long long total = 0;
    std::string line;

    // Read each line, split on ',' and then '-' to obtain the low/high pairs.
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;

        while (std::getline(ss, token, ',')) {
            std::stringstream rangeStream(token);
            std::string lowStr, highStr;
            if (!std::getline(rangeStream, lowStr, '-')) continue;
            if (!std::getline(rangeStream, highStr, '-')) continue;

            long long low = std::stoll(lowStr);
            long long high = std::stoll(highStr);

            int digitsLow = numDigits(low);
            int digitsHigh = numDigits(high);

            if (digitsLow == digitsHigh) {
                if (digitsLow % 2 == 0) {
                    total += sumDoubleNumbers(low, high);
                }
                continue;
            }

            if (std::abs(digitsLow - digitsHigh) == 1) {
                // Keep only the even-digit portion of the interval.
                if (digitsLow % 2 == 0) {
                    long long adjustedHigh = pow10(digitsLow) - 1;
                    if (adjustedHigh >= low) {
                        total += sumDoubleNumbers(low, std::min(high, adjustedHigh));
                    }
                } else if (digitsHigh % 2 == 0) {
                    long long adjustedLow = pow10(digitsHigh - 1);
                    if (adjustedLow <= high) {
                        total += sumDoubleNumbers(std::max(low, adjustedLow), high);
                    }
                } else {
                    continue;
                }
            } else {
                std::cerr << "Unexpected digit gap for range " << low << "-" << high << '\n';
            }
        }
    }

    std::cout << total << std::endl;
    return 0;
}
