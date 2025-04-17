#include <array>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

/*
 * Lightweight MD5 implementation (public‑domain, abridged for clarity).
 * Based on the RSA Data Security, Inc. MD5 Message‑Digest Algorithm.
 * Only the interface needed for this task is retained.
 */
class MD5 {
public:
    explicit MD5(const std::string& text) { init(); update(text); finalize(); }

    std::string hex() const {
        std::ostringstream out;
        out << std::hex << std::setfill('0');
        for (auto b: digest_) out << std::setw(2) << static_cast<int>(b);
        return out.str();
    }

private:
    void init() {
        bit_len_ = 0;
        state_[0] = 0x67452301u;
        state_[1] = 0xefcdab89u;
        state_[2] = 0x98badcfeu;
        state_[3] = 0x10325476u;
    }

    static uint32_t left_rotate(uint32_t x, uint32_t c) { return (x << c) | (x >> (32 - c)); }

    void transform(const uint8_t block[64]) {
        static constexpr uint32_t k[64] = {
            0xd76aa478u, 0xe8c7b756u, 0x242070dbu, 0xc1bdceeeu, 0xf57c0fafu, 0x4787c62au,
            0xa8304613u, 0xfd469501u, 0x698098d8u, 0x8b44f7afu, 0xffff5bb1u, 0x895cd7beu,
            0x6b901122u, 0xfd987193u, 0xa679438eu, 0x49b40821u, 0xf61e2562u, 0xc040b340u,
            0x265e5a51u, 0xe9b6c7aau, 0xd62f105du, 0x02441453u, 0xd8a1e681u, 0xe7d3fbc8u,
            0x21e1cde6u, 0xc33707d6u, 0xf4d50d87u, 0x455a14edu, 0xa9e3e905u, 0xfcefa3f8u,
            0x676f02d9u, 0x8d2a4c8au, 0xfffa3942u, 0x8771f681u, 0x6d9d6122u, 0xfde5380cu,
            0xa4beea44u, 0x4bdecfa9u, 0xf6bb4b60u, 0xbebfbc70u, 0x289b7ec6u, 0xeaa127fau,
            0xd4ef3085u, 0x04881d05u, 0xd9d4d039u, 0xe6db99e5u, 0x1fa27cf8u, 0xc4ac5665u,
            0xf4292244u, 0x432aff97u, 0xab9423a7u, 0xfc93a039u, 0x655b59c3u, 0x8f0ccc92u,
            0xffeff47du, 0x85845dd1u, 0x6fa87e4fu, 0xfe2ce6e0u, 0xa3014314u, 0x4e0811a1u,
            0xf7537e82u, 0xbd3af235u, 0x2ad7d2bbu, 0xeb86d391u
        };
        static constexpr uint32_t r[64] = {
            7u, 12u, 17u, 22u, 7u, 12u, 17u, 22u, 7u, 12u, 17u, 22u, 7u, 12u, 17u, 22u,
            5u, 9u, 14u, 20u, 5u, 9u, 14u, 20u, 5u, 9u, 14u, 20u, 5u, 9u, 14u, 20u,
            4u, 11u, 16u, 23u, 4u, 11u, 16u, 23u, 4u, 11u, 16u, 23u, 4u, 11u, 16u, 23u,
            6u, 10u, 15u, 21u, 6u, 10u, 15u, 21u, 6u, 10u, 15u, 21u, 6u, 10u, 15u, 21u
        };

        uint32_t a = state_[0], b = state_[1], c = state_[2], d = state_[3], f, g;
        uint32_t m[16];
        std::memcpy(m, block, 64);

        for (uint32_t i = 0; i < 64; ++i) {
            if (i < 16) {
                f = (b & c) | (~b & d);
                g = i;
            } else if (i < 32) {
                f = (d & b) | (~d & c);
                g = (5 * i + 1) & 15;
            } else if (i < 48) {
                f = b ^ c ^ d;
                g = (3 * i + 5) & 15;
            } else {
                f = c ^ (b | ~d);
                g = (7 * i) & 15;
            }
            uint32_t temp = d;
            d = c;
            c = b;
            b = b + left_rotate(a + f + k[i] + m[g], r[i]);
            a = temp;
        }
        state_[0] += a;
        state_[1] += b;
        state_[2] += c;
        state_[3] += d;
    }

    void update(const std::string& input) {
        for (unsigned char byte : input) add(byte);
    }

    void add(uint8_t byte) {
        buffer_[buffer_len_++] = byte;
        bit_len_ += 8;
        if (buffer_len_ == 64) {
            transform(buffer_.data());
            buffer_len_ = 0;
        }
    }

    void finalize() {
        buffer_[buffer_len_++] = 0x80;
        if (buffer_len_ > 56) {
            while (buffer_len_ < 64) buffer_[buffer_len_++] = 0;
            transform(buffer_.data());
            buffer_len_ = 0;
        }
        while (buffer_len_ < 56) buffer_[buffer_len_++] = 0;
        uint64_t bit_len_le = bit_len_;
        std::memcpy(&buffer_[56], &bit_len_le, 8);
        transform(buffer_.data());
        for (int i = 0; i < 4; ++i)
            std::memcpy(&digest_[i * 4], &state_[i], 4);
    }

    uint64_t bit_len_{0};
    uint32_t state_[4]{};
    std::array<uint8_t, 64> buffer_{};
    size_t buffer_len_{0};
    std::array<uint8_t, 16> digest_{};
};

/* ---------- Helper functions for the puzzle ---------- */

// Return the first character that appears three times consecutively; 0 if none.
char first_triplet(const std::string& s) {
    for (std::size_t i = 2; i < s.size(); ++i)
        if (s[i] == s[i - 1] && s[i] == s[i - 2]) return s[i];
    return 0;
}

// Does the string contain the given character five times in a row?
bool has_quintuple(const std::string& s, char c) {
    std::array<char, 5> pattern{};
    pattern.fill(c);
    auto it = std::search(s.begin(), s.end(), pattern.begin(), pattern.end());
    return it != s.end();
}

// Compute a stretched hash (one initial MD5 + 2016 additional).
std::string stretched_hash(const std::string& salt, int index) {
    static std::unordered_map<int, std::string> cache;
    auto it = cache.find(index);
    if (it != cache.end()) return it->second;

    std::string h = MD5(salt + std::to_string(index)).hex();
    for (int i = 0; i < 2016; ++i) h = MD5(h).hex();
    cache.emplace(index, h);
    return h;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string salt;
    if (!(std::cin >> salt)) return 0;

    static constexpr int target_keys = 64;
    const int lookahead = 1000;

    int keys_found = 0;
    int index = 0;
    int answer = -1;

    while (keys_found < target_keys) {
        const std::string& hash = stretched_hash(salt, index);
        if (char c = first_triplet(hash); c != 0) {
            bool valid = false;
            for (int j = index + 1; j <= index + lookahead; ++j) {
                if (has_quintuple(stretched_hash(salt, j), c)) {
                    valid = true;
                    break;
                }
            }
            if (valid) {
                ++keys_found;
                if (keys_found == target_keys) {
                    answer = index;
                    break;
                }
            }
        }
        ++index;
    }

    std::cout << answer << std::endl;
    return 0;
}
