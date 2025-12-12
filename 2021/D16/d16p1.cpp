#include <iostream>
#include <string>
#include <cstdint>

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

static std::uint64_t parse_packet(BitStream& bs) {
    std::uint64_t version_sum = bs.read(3);
    const std::uint64_t type = bs.read(3);

    if (type == 4) {
        while (true) {
            const std::uint64_t more = bs.read(1);
            bs.read(4);
            if (more == 0) break;
        }
        return version_sum;
    }

    const std::uint64_t len_type = bs.read(1);
    if (len_type == 0) {
        const std::uint64_t total_bits = bs.read(15);
        const std::size_t end = bs.pos + static_cast<std::size_t>(total_bits);
        while (bs.pos < end) version_sum += parse_packet(bs);
        return version_sum;
    }

    const std::uint64_t sub_count = bs.read(11);
    for (std::uint64_t i = 0; i < sub_count; i++) version_sum += parse_packet(bs);
    return version_sum;
}

int main() {
    std::string hex;
    if (!(std::cin >> hex)) return 0;
    BitStream bs(hex);
    std::cout << parse_packet(bs) << std::endl;
    return 0;
}

