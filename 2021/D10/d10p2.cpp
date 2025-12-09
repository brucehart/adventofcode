#include <algorithm>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::unordered_map<char, char> match = {{')', '('}, {']', '['}, {'}', '{'}, {'>', '<'}};
    std::unordered_map<char, char> closing = {{'(', ')'}, {'[', ']'}, {'{', '}'}, {'<', '>'}};
    std::unordered_map<char, int> score = {{')', 1}, {']', 2}, {'}', 3}, {'>', 4}};

    std::string line;
    std::vector<long long> scores;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::stack<char> st;
        bool corrupted = false;

        for (char c : line) {
            if (c == '(' || c == '[' || c == '{' || c == '<') {
                st.push(c);
            } else {
                if (st.empty() || st.top() != match[c]) {
                    corrupted = true;
                    break;
                }
                st.pop();
            }
        }

        if (corrupted || st.empty()) continue;

        long long total = 0;
        while (!st.empty()) {
            char open = st.top();
            st.pop();
            char close = closing[open];
            total = total * 5 + score[close];
        }
        scores.push_back(total);
    }

    if (scores.empty()) return 0;
    std::sort(scores.begin(), scores.end());
    std::cout << scores[scores.size() / 2] << std::endl;
    return 0;
}
