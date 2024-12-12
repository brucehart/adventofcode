#include <iostream>
#include <string>
#include <algorithm>

int main() {
    std::string line;
    int total_code_chars = 0;
    int total_memory_chars = 0;

    while (std::getline(std::cin, line)) {
        total_code_chars += line.length();

        int memory_chars = 0;
        for (size_t i = 1; i < line.length() - 1; ++i) {
            if (line[i] == '\\') {
                if (line[i + 1] == '\\' || line[i + 1] == '\"') {
                    memory_chars++;
                    i++;
                } else if (line[i + 1] == 'x') {
                    memory_chars++;
                    i += 3;
                }
            } else {
                memory_chars++;
            }
        }
        total_memory_chars += memory_chars;
    }

    std::cout << total_code_chars - total_memory_chars << std::endl;

    return 0;
}
