#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

int main() {
    std::string template_str;
    std::unordered_map<std::string, char> ins;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        template_str = line;
        break;
    }
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        if (line.size() < 6) continue;
        const std::string pair = line.substr(0, 2);
        const char ch = line.back();
        ins[pair] = ch;
    }
    if (template_str.empty()) return 0;

    std::string cur = template_str;
    for (int step = 0; step < 10; step++) {
        std::string next;
        next.reserve(cur.size() * 2);
        for (std::size_t i = 0; i + 1 < cur.size(); i++) {
            next.push_back(cur[i]);
            std::string pair;
            pair.push_back(cur[i]);
            pair.push_back(cur[i + 1]);
            auto it = ins.find(pair);
            if (it != ins.end()) next.push_back(it->second);
        }
        next.push_back(cur.back());
        cur.swap(next);
    }

    std::vector<long long> cnt(256, 0);
    for (unsigned char c : cur) cnt[c]++;

    long long mn = 0, mx = 0;
    for (long long v : cnt) {
        if (!v) continue;
        if (mn == 0 || v < mn) mn = v;
        if (v > mx) mx = v;
    }

    std::cout << (mx - mn) << std::endl;
    return 0;
}

