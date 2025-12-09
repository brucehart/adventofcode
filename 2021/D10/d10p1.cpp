#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>

int main() {
    std::unordered_map<char, char> match = {{')', '('}, {']', '['}, {'}', '{'}, {'>', '<'}};
    std::unordered_map<char, int> score = {{')', 3}, {']', 57}, {'}', 1197}, {'>', 25137}};

    std::string line;
    long long total = 0;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::stack<char> st;
        for (char c : line) {
            if (c == '(' || c == '[' || c == '{' || c == '<') {
                st.push(c);
            } else {
                if (st.empty() || st.top() != match[c]) {
                    total += score[c];
                    break;
                }
                st.pop();
            }
        }
    }

    std::cout << total << std::endl;
    return 0;
}
