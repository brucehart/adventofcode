#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

static int pair_idx(unsigned char a, unsigned char b) {
    return (static_cast<int>(a) << 8) | static_cast<int>(b);
}

int main() {
    std::string tmpl;
    std::vector<int> ins(256 * 256, -1);

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        tmpl = line;
        break;
    }
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        if (line.size() < 6) continue;
        const unsigned char a = static_cast<unsigned char>(line[0]);
        const unsigned char b = static_cast<unsigned char>(line[1]);
        const unsigned char c = static_cast<unsigned char>(line.back());
        ins[pair_idx(a, b)] = static_cast<int>(c);
    }
    if (tmpl.empty()) return 0;

    std::vector<long long> pair_cnt(256 * 256, 0);
    std::vector<long long> char_cnt(256, 0);

    for (unsigned char c : tmpl) char_cnt[c]++;
    for (std::size_t i = 0; i + 1 < tmpl.size(); i++) {
        const unsigned char a = static_cast<unsigned char>(tmpl[i]);
        const unsigned char b = static_cast<unsigned char>(tmpl[i + 1]);
        pair_cnt[pair_idx(a, b)]++;
    }

    for (int step = 0; step < 40; step++) {
        std::vector<long long> next(256 * 256, 0);
        for (int i = 0; i < 256 * 256; i++) {
            const long long n = pair_cnt[i];
            if (!n) continue;

            const int c = ins[i];
            if (c < 0) {
                next[i] += n;
                continue;
            }

            const unsigned char a = static_cast<unsigned char>(i >> 8);
            const unsigned char b = static_cast<unsigned char>(i & 0xff);
            const unsigned char mid = static_cast<unsigned char>(c);

            char_cnt[mid] += n;
            next[pair_idx(a, mid)] += n;
            next[pair_idx(mid, b)] += n;
        }
        pair_cnt.swap(next);
    }

    long long mn = 0, mx = 0;
    for (long long v : char_cnt) {
        if (!v) continue;
        if (mn == 0 || v < mn) mn = v;
        if (v > mx) mx = v;
    }

    std::cout << (mx - mn) << std::endl;
    return 0;
}

