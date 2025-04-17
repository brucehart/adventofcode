#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstdint>
#include <cstring>

// Public-domain C++ MD5 implementation (based on RFC 1321)
// Simplified for clarity and single-file use
class MD5 {
public:
    MD5() { init(); }

    // Compute MD5 of input string and return lowercase hex digest
    static std::string md5(const std::string& input) {
        MD5 ctx;
        ctx.update(reinterpret_cast<const uint8_t*>(input.data()), input.size());
        ctx.finalize();
        return ctx.hexdigest();
    }

    void update(const uint8_t* input, size_t length) {
        size_t i = bufferLen;
        bufferLen = (bufferLen + length) % 64;
        count[0] += static_cast<uint32_t>(length << 3);
        if (count[0] < (length << 3)) count[1]++;
        count[1] += static_cast<uint32_t>(length >> 29);

        size_t partLen = 64 - i;
        size_t idx = 0;
        if (length >= partLen) {
            std::memcpy(buffer + i, input, partLen);
            transform(buffer);
            for (idx = partLen; idx + 63 < length; idx += 64)
                transform(input + idx);
            i = 0;
        }
        std::memcpy(buffer + i, input + idx, length - idx);
    }

    void finalize() {
        static const uint8_t padding[64] = { 0x80 };
        if (finalized) return;

        uint8_t bits[8];
        encode(bits, count, 8);

        size_t padLen = (bufferLen < 56) ? (56 - bufferLen) : (120 - bufferLen);
        update(padding, padLen);
        update(bits, 8);

        encode(digest, state, 16);
        finalized = true;
    }

    std::string hexdigest() const {
        std::ostringstream ss;
        ss << std::hex << std::setfill('0');
        for (uint8_t byte : digest)
            ss << std::setw(2) << static_cast<int>(byte);
        return ss.str();
    }

private:
    void init() {
        finalized = false;
        bufferLen = 0;
        count[0] = count[1] = 0;
        // Per RFC 1321
        state[0] = 0x67452301;
        state[1] = 0xefcdab89;
        state[2] = 0x98badcfe;
        state[3] = 0x10325476;
    }

    void transform(const uint8_t block[64]) {
        static constexpr uint32_t T[64] = {
            0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
            0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
            0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
            0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
            0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
            0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
            0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
            0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
            0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
            0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
            0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
            0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
            0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
            0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
            0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
            0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
        };
        static const uint32_t s[64] = {
             7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
             5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
             4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
             6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
        };

        uint32_t X[16];
        decode(X, block, 64);

        uint32_t A = state[0], B = state[1], C = state[2], D = state[3];

        for (int i = 0; i < 64; ++i) {
            uint32_t F, g;
            if (i < 16) {
                F = (B & C) | (~B & D);
                g = i;
            } else if (i < 32) {
                F = (D & B) | (~D & C);
                g = (5 * i + 1) % 16;
            } else if (i < 48) {
                F = B ^ C ^ D;
                g = (3 * i + 5) % 16;
            } else {
                F = C ^ (B | ~D);
                g = (7 * i) % 16;
            }
            uint32_t tmp = D;
            D = C;
            C = B;
            B = B + leftRotate(A + F + T[i] + X[g], s[i]);
            A = tmp;
        }

        state[0] += A;
        state[1] += B;
        state[2] += C;
        state[3] += D;
    }

    static uint32_t leftRotate(uint32_t x, uint32_t c) {
        return (x << c) | (x >> (32 - c));
    }

    static void encode(uint8_t output[], const uint32_t input[], size_t len) {
        for (size_t i = 0, j = 0; j < len; ++i, j += 4) {
            output[j]     = static_cast<uint8_t>( input[i]        & 0xff);
            output[j + 1] = static_cast<uint8_t>((input[i] >> 8)  & 0xff);
            output[j + 2] = static_cast<uint8_t>((input[i] >> 16) & 0xff);
            output[j + 3] = static_cast<uint8_t>((input[i] >> 24) & 0xff);
        }
    }

    static void decode(uint32_t output[], const uint8_t input[], size_t len) {
        for (size_t i = 0, j = 0; j < len; ++i, j += 4) {
            output[i] =  static_cast<uint32_t>(input[j])
                       | (static_cast<uint32_t>(input[j + 1]) << 8)
                       | (static_cast<uint32_t>(input[j + 2]) << 16)
                       | (static_cast<uint32_t>(input[j + 3]) << 24);
        }
    }

    bool finalized = false;
    uint8_t buffer[64];
    size_t bufferLen = 0;
    uint32_t count[2];
    uint32_t state[4];
    uint8_t digest[16];
};

int main() {
    std::string salt;
    if (!(std::cin >> salt)) {
        return 0;
    }

    // Cache of computed hashes for on-demand reuse
    std::vector<std::string> cache;
    cache.reserve(50000);

    auto getHash = [&](int idx) -> const std::string& {
        if (idx >= static_cast<int>(cache.size())) {
            for (int i = static_cast<int>(cache.size()); i <= idx; ++i) {
                cache.push_back(MD5::md5(salt + std::to_string(i)));
            }
        }
        return cache[idx];
    };

    int foundKeys = 0;
    int index = 0;

    while (true) {
        const std::string& h = getHash(index);

        // Find first triplet in h
        char tripleChar = 0;
        for (size_t i = 0; i + 2 < h.size(); ++i) {
            if (h[i] == h[i+1] && h[i] == h[i+2]) {
                tripleChar = h[i];
                break;
            }
        }

        if (tripleChar != 0) {
            // Look for five in a row in the next 1000 hashes
            std::string quint(5, tripleChar);
            bool hasQuint = false;
            for (int j = index + 1; j <= index + 1000; ++j) {
                if (getHash(j).find(quint) != std::string::npos) {
                    hasQuint = true;
                    break;
                }
            }
            if (hasQuint) {
                ++foundKeys;
                if (foundKeys == 64) {
                    std::cout << index << std::endl;
                    break;
                }
            }
        }

        ++index;
    }

    return 0;
}
