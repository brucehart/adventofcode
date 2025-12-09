#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main() {
    std::string line;
    long long count = 0;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        bool outputs = false;
        while (ss >> token) {
            if (token == "|") {
                outputs = true;
                continue;
            }
            if (!outputs) continue;

            const int len = token.size();
            if (len == 2 || len == 3 || len == 4 || len == 7) count++;
        }
    }

    std::cout << count << std::endl;
    return 0;
}
