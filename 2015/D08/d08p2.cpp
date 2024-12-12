#include <iostream>
#include <string>
#include <algorithm>

int main() {
    std::string line;
    int total_original_chars = 0;
    int total_encoded_chars = 0;

    while (std::getline(std::cin, line)) {
        total_original_chars += line.length();

        int encoded_chars = 2; // Start with 2 for the surrounding quotes
        for (char c : line) {
            if (c == '\"' || c == '\\') {
                encoded_chars += 2; // Escape double quotes and backslashes
            } else {
                encoded_chars += 1;
            }
        }
        total_encoded_chars += encoded_chars;
    }

    std::cout << total_encoded_chars - total_original_chars << std::endl;

    return 0;
}
