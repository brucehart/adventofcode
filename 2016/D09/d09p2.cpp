#include <iostream>
#include <string>
#include <algorithm>

long long decompressedLength(const std::string& compressed) {
    long long length = 0;
    for (size_t i = 0; i < compressed.length(); ++i) {
        if (compressed[i] == '(') {
            size_t xPos = compressed.find('x', i);
            size_t closePos = compressed.find(')', i);
            int charCount = std::stoi(compressed.substr(i + 1, xPos - i - 1));
            int repeatCount = std::stoi(compressed.substr(xPos + 1, closePos - xPos - 1));

            std::string repeatedPart = compressed.substr(closePos + 1, charCount);
            length += decompressedLength(repeatedPart) * repeatCount;
            i = closePos + charCount; // Skip the repeated characters and the marker
        } else if (compressed[i] != ' ') { // Ignore whitespace
            length++;
        }
    }
    return length;
}

int main() {
    std::string compressed;
    std::getline(std::cin, compressed);

    std::cout << decompressedLength(compressed) << std::endl;

    return 0;
}
