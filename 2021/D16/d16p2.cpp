#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>

struct BitStream {
    std::string bits;
    std::size_t pos = 0;

    static int hex_val(char c) {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
        if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
        return 0;
    }

    explicit BitStream(const std::string& hex) {
        bits.reserve(hex.size() * 4);
        for (char c : hex) {
            if (c == '\n' || c == '\r' || c == ' ' || c == '\t') continue;
            const int v = hex_val(c);
            for (int k = 3; k >= 0; k--) bits.push_back(((v >> k) & 1) ? '1' : '0');
        }
    }

    std::uint64_t read(int n) {
        std::uint64_t v = 0;
        for (int i = 0; i < n; i++) {
            v = (v << 1) | (pos < bits.size() && bits[pos] == '1' ? 1ULL : 0ULL);
            pos++;
        }
        return v;
    }
};

static std::uint64_t parse_value(BitStream& bs) {
    bs.read(3); // version (ignored for part 2)
    const std::uint64_t type = bs.read(3);

    if (type == 4) {
        std::uint64_t v = 0;
        while (true) {
            const std::uint64_t more = bs.read(1);
            const std::uint64_t group = bs.read(4);
            v = (v << 4) | group;
            if (more == 0) break;
        }
        return v;
    }

    std::vector<std::uint64_t> vals;
    const std::uint64_t len_type = bs.read(1);
    if (len_type == 0) {
        const std::uint64_t total_bits = bs.read(15);
        const std::size_t end = bs.pos + static_cast<std::size_t>(total_bits);
        while (bs.pos < end) vals.push_back(parse_value(bs));
    } else {
        const std::uint64_t sub_count = bs.read(11);
        vals.reserve(static_cast<std::size_t>(sub_count));
        for (std::uint64_t i = 0; i < sub_count; i++) vals.push_back(parse_value(bs));
    }

    if (type == 0) {
        std::uint64_t sum = 0;
        for (auto v : vals) sum += v;
        return sum;
    }
    if (type == 1) {
        std::uint64_t prod = 1;
        for (auto v : vals) prod *= v;
        return prod;
    }
    if (type == 2) return *std::min_element(vals.begin(), vals.end());
    if (type == 3) return *std::max_element(vals.begin(), vals.end());
    if (type == 5) return vals[0] > vals[1] ? 1 : 0;
    if (type == 6) return vals[0] < vals[1] ? 1 : 0;
    if (type == 7) return vals[0] == vals[1] ? 1 : 0;
    return 0;
}

int main() {
    std::string hex;
    if (!(std::cin >> hex)) return 0;
    BitStream bs(hex);
    std::cout << parse_value(bs) << std::endl;
    return 0;
}

