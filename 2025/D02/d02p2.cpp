#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using int128 = __int128_t;

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

std::string toString(int128 value) {
    if (value == 0) return "0";
    bool negative = value < 0;
    if (negative) value = -value;

    std::string digits;
    while (value > 0) {
        int digit = static_cast<int>(value % 10);
        digits.push_back(static_cast<char>('0' + digit));
        value /= 10;
    }

    if (negative) digits.push_back('-');
    std::reverse(digits.begin(), digits.end());
    return digits;
}

int main() {
    std::string line;
    std::vector<std::pair<long long, long long>> ranges;

    // Parse all ranges up front.
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;

        while (std::getline(ss, token, ',')) {
            if (token.empty()) continue;
            auto dashPos = token.find('-');
            if (dashPos == std::string::npos) continue;

            long long low = std::stoll(token.substr(0, dashPos));
            long long high = std::stoll(token.substr(dashPos + 1));
            ranges.emplace_back(low, high);
        }
    }

    // Precompute powers of 10 up to 10^19 (covers all 64-bit positive numbers).
    std::vector<int128> pow10(20, 1);
    for (std::size_t i = 1; i < pow10.size(); ++i) pow10[i] = pow10[i - 1] * 10;

    auto ceilDiv = [](int128 a, int128 b) { return (a + b - 1) / b; };

    int128 total = 0;

    for (const auto& [low, high] : ranges) {
        int maxDigits = numDigits(high);

        // unique[len][repeat] holds the contribution of numbers whose minimal block length is exactly len.
        std::vector<std::vector<int128>> unique(maxDigits + 1);
        for (int len = 1; len <= maxDigits; ++len) {
            int maxRepeat = maxDigits / len;
            unique[len].assign(maxRepeat + 1, 0);
        }

        // Precompute proper divisors for each length to speed up overlap subtraction.
        std::vector<std::vector<int>> divisors(maxDigits + 1);
        for (int len = 1; len <= maxDigits; ++len) {
            for (int d = 1; d * d <= len; ++d) {
                if (len % d != 0) continue;
                if (d != len) divisors[len].push_back(d);
                int other = len / d;
                if (other != d && other != len) divisors[len].push_back(other);
            }
        }

        for (int blockLen = 1; blockLen <= maxDigits; ++blockLen) {
            int maxRepeat = maxDigits / blockLen;

            int128 minBlock = pow10[blockLen - 1];
            int128 maxBlock = pow10[blockLen] - 1;
            int128 blockDivisor = pow10[blockLen] - 1;

            for (int repeat = 2; repeat <= maxRepeat; ++repeat) {
                int digits = blockLen * repeat;

                int128 repFactor = (pow10[digits] - 1) / blockDivisor;

                int128 start = ceilDiv(static_cast<int128>(low), repFactor);
                if (start < minBlock) start = minBlock;

                int128 end = static_cast<int128>(high) / repFactor;
                if (end > maxBlock) end = maxBlock;

                if (start > end) continue;

                int128 count = end - start + 1;
                int128 sumBlocks = (start + end) * count / 2;
                int128 rawSum = sumBlocks * repFactor;

                int128 overlap = 0;
                for (int d : divisors[blockLen]) {
                    int newRepeat = repeat * (blockLen / d);
                    overlap += unique[d][newRepeat];
                }

                int128 uniqueContribution = rawSum - overlap;
                unique[blockLen][repeat] = uniqueContribution;
                total += uniqueContribution;
            }
        }
    }

    std::cout << toString(total) << '\n';
    return 0;
}
