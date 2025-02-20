#include <iostream>
#include <string>
#include <algorithm>

int main() {
    std::string compressed;
    std::getline(std::cin, compressed);

    long long decompressedLength = 0;
    size_t i = 0;
    while (i < compressed.length()) {
        if (compressed[i] == '(') {
            size_t xPos = compressed.find('x', i);
            size_t closePos = compressed.find(')', i);
            int charCount = std::stoi(compressed.substr(i + 1, xPos - i - 1));
            int repeatCount = std::stoi(compressed.substr(xPos + 1, closePos - xPos - 1));

            decompressedLength += static_cast<long long>(charCount) * repeatCount;
            i = closePos + 1 + charCount; // Skip the repeated characters
        } else if (compressed[i] != ' ') { // Ignore whitespace
            decompressedLength++;
            i++;
        } else {
            i++; // Skip whitespace
        }
    }

    std::cout << decompressedLength << std::endl;

    return 0;
}
