#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

// Function to convert a SNAFU character to its decimal value
int snafuCharToValue(char c) {
    switch (c) {
        case '2': return 2;
        case '1': return 1;
        case '0': return 0;
        case '-': return -1;
        case '=': return -2;
        default:
            // Invalid character encountered
            std::cerr << "Invalid SNAFU character: " << c << std::endl;
            exit(1);
    }
}

// Function to convert a SNAFU string to its decimal equivalent
long long snafuToDecimal(const std::string& snafu) {
    long long decimal = 0;
    long long power = 1; // 5^0 initially

    // Iterate from the end (least significant digit) to the beginning
    for (auto it = snafu.rbegin(); it != snafu.rend(); ++it) {
        int digit = snafuCharToValue(*it);
        decimal += digit * power;
        power *= 5;
    }

    return decimal;
}

// Function to map a decimal digit to its corresponding SNAFU character
char valueToSnafuChar(int value) {
    switch (value) {
        case 2: return '2';
        case 1: return '1';
        case 0: return '0';
        case -1: return '-';
        case -2: return '=';
        default:
            // Invalid value encountered
            std::cerr << "Invalid value for SNAFU digit: " << value << std::endl;
            exit(1);
    }
}

// Function to convert a decimal number to its SNAFU string equivalent
std::string decimalToSnafu(long long decimal) {
    if (decimal == 0) return "0";

    std::string snafu;
    while (decimal != 0) {
        int remainder = decimal % 5;
        decimal /= 5;

        if (remainder > 2) {
            remainder -= 5;
            decimal += 1;
        }

        snafu += valueToSnafuChar(remainder);
    }

    // The digits were generated in reverse order, so reverse them back
    std::reverse(snafu.begin(), snafu.end());
    return snafu;
}

int main() {
    std::string line;
    long long total = 0;

    // Read each SNAFU number from standard input
    while (std::getline(std::cin, line)) {
        if (!line.empty()) {
            total += snafuToDecimal(line);
        }
    }

    // Convert the total sum back to SNAFU and output
    std::cout << decimalToSnafu(total) << std::endl;

    return 0;
}

