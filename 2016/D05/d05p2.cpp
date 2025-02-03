#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <cstdint>

// Minimal MD5 implementation.
// This class implements the MD5 message digest algorithm.
class MD5 {
public:
    MD5() { init(); }

    // Update the MD5 context with a block of data.
    void update(const unsigned char* input, std::size_t length) {
        // Compute number of bytes mod 64.
        std::size_t index = (count[0] >> 3) & 0x3F;

        // Update number of bits.
        if ((count[0] += (static_cast<uint32_t>(length) << 3)) < (length << 3))
            ++count[1];
        count[1] += (static_cast<uint32_t>(length) >> 29);

        std::size_t partLen = 64 - index;
        std::size_t i = 0;

        // Transform as many times as possible.
        if (length >= partLen) {
            std::memcpy(&buffer[index], input, partLen);
            transform(buffer);
            for (i = partLen; i + 63 < length; i += 64)
                transform(&input[i]);
            index = 0;
        }
        // Buffer remaining input.
        std::memcpy(&buffer[index], &input[i], length - i);
    }

    // Overloaded update function to accept a char pointer.
    void update(const char* input, std::size_t length) {
        update(reinterpret_cast<const unsigned char*>(input), length);
    }

    // Finalize the MD5 computation and return the digest as a hexadecimal string.
    std::string final() {
        static unsigned char padding[64] = { 0x80 };
        unsigned char bits[8];
        encode(count, bits, 8);

        // Pad to 56 mod 64.
        std::size_t index = (count[0] >> 3) & 0x3F;
        std::size_t padLen = (index < 56) ? (56 - index) : (120 - index);
        update(padding, padLen);

        // Append length (before padding).
        update(bits, 8);

        // Encode state into digest.
        encode(state, digest, 16);

        // Convert digest to hexadecimal string.
        std::ostringstream result;
        for (int i = 0; i < 16; ++i)
            result << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
        return result.str();
    }

private:
    void init() {
        finalized = false;
        count[0] = count[1] = 0;
        // Magic initialization constants.
        state[0] = 0x67452301;
        state[1] = 0xefcdab89;
        state[2] = 0x98badcfe;
        state[3] = 0x10325476;
    }

    // The core MD5 transformation that processes 64-byte blocks.
    void transform(const unsigned char block[64]) {
        uint32_t a = state[0], b = state[1], c = state[2], d = state[3], x[16];
        decode(block, x, 64);

        auto F = [](uint32_t x, uint32_t y, uint32_t z) -> uint32_t { return (x & y) | (~x & z); };
        auto G = [](uint32_t x, uint32_t y, uint32_t z) -> uint32_t { return (x & z) | (y & ~z); };
        auto H = [](uint32_t x, uint32_t y, uint32_t z) -> uint32_t { return x ^ y ^ z; };
        auto I = [](uint32_t x, uint32_t y, uint32_t z) -> uint32_t { return y ^ (x | ~z); };
        auto rotate_left = [](uint32_t x, int n) -> uint32_t { return (x << n) | (x >> (32 - n)); };

        // Round 1.
        #define FF(a,b,c,d,x,s,ac) { \
            a = a + F(b,c,d) + x + ac; \
            a = rotate_left(a, s); \
            a += b; \
        }
        // Round 2.
        #define GG(a,b,c,d,x,s,ac) { \
            a = a + G(b,c,d) + x + ac; \
            a = rotate_left(a, s); \
            a += b; \
        }
        // Round 3.
        #define HH(a,b,c,d,x,s,ac) { \
            a = a + H(b,c,d) + x + ac; \
            a = rotate_left(a, s); \
            a += b; \
        }
        // Round 4.
        #define II(a,b,c,d,x,s,ac) { \
            a = a + I(b,c,d) + x + ac; \
            a = rotate_left(a, s); \
            a += b; \
        }

        // Round 1
        FF(a, b, c, d, x[ 0], 7, 0xd76aa478);
        FF(d, a, b, c, x[ 1], 12, 0xe8c7b756);
        FF(c, d, a, b, x[ 2], 17, 0x242070db);
        FF(b, c, d, a, x[ 3], 22, 0xc1bdceee);
        FF(a, b, c, d, x[ 4], 7, 0xf57c0faf);
        FF(d, a, b, c, x[ 5], 12, 0x4787c62a);
        FF(c, d, a, b, x[ 6], 17, 0xa8304613);
        FF(b, c, d, a, x[ 7], 22, 0xfd469501);
        FF(a, b, c, d, x[ 8], 7, 0x698098d8);
        FF(d, a, b, c, x[ 9], 12, 0x8b44f7af);
        FF(c, d, a, b, x[10], 17, 0xffff5bb1);
        FF(b, c, d, a, x[11], 22, 0x895cd7be);
        FF(a, b, c, d, x[12], 7, 0x6b901122);
        FF(d, a, b, c, x[13], 12, 0xfd987193);
        FF(c, d, a, b, x[14], 17, 0xa679438e);
        FF(b, c, d, a, x[15], 22, 0x49b40821);

        // Round 2
        GG(a, b, c, d, x[ 1], 5, 0xf61e2562);
        GG(d, a, b, c, x[ 6], 9, 0xc040b340);
        GG(c, d, a, b, x[11], 14, 0x265e5a51);
        GG(b, c, d, a, x[ 0], 20, 0xe9b6c7aa);
        GG(a, b, c, d, x[ 5], 5, 0xd62f105d);
        GG(d, a, b, c, x[10], 9,  0x2441453);
        GG(c, d, a, b, x[15], 14, 0xd8a1e681);
        GG(b, c, d, a, x[ 4], 20, 0xe7d3fbc8);
        GG(a, b, c, d, x[ 9], 5, 0x21e1cde6);
        GG(d, a, b, c, x[14], 9, 0xc33707d6);
        GG(c, d, a, b, x[ 3], 14, 0xf4d50d87);
        GG(b, c, d, a, x[ 8], 20, 0x455a14ed);
        GG(a, b, c, d, x[13], 5, 0xa9e3e905);
        GG(d, a, b, c, x[ 2], 9, 0xfcefa3f8);
        GG(c, d, a, b, x[ 7], 14, 0x676f02d9);
        GG(b, c, d, a, x[12], 20, 0x8d2a4c8a);

        // Round 3
        HH(a, b, c, d, x[ 5], 4, 0xfffa3942);
        HH(d, a, b, c, x[ 8], 11, 0x8771f681);
        HH(c, d, a, b, x[11], 16, 0x6d9d6122);
        HH(b, c, d, a, x[14], 23, 0xfde5380c);
        HH(a, b, c, d, x[ 1], 4, 0xa4beea44);
        HH(d, a, b, c, x[ 4], 11, 0x4bdecfa9);
        HH(c, d, a, b, x[ 7], 16, 0xf6bb4b60);
        HH(b, c, d, a, x[10], 23, 0xbebfbc70);
        HH(a, b, c, d, x[13], 4, 0x289b7ec6);
        HH(d, a, b, c, x[ 0], 11, 0xeaa127fa);
        HH(c, d, a, b, x[ 3], 16, 0xd4ef3085);
        HH(b, c, d, a, x[ 6], 23,  0x4881d05);
        HH(a, b, c, d, x[ 9], 4, 0xd9d4d039);
        HH(d, a, b, c, x[12], 11, 0xe6db99e5);
        HH(c, d, a, b, x[15], 16, 0x1fa27cf8);
        HH(b, c, d, a, x[ 2], 23, 0xc4ac5665);

        // Round 4
        II(a, b, c, d, x[ 0], 6, 0xf4292244);
        II(d, a, b, c, x[ 7], 10, 0x432aff97);
        II(c, d, a, b, x[14], 15, 0xab9423a7);
        II(b, c, d, a, x[ 5], 21, 0xfc93a039);
        II(a, b, c, d, x[12], 6, 0x655b59c3);
        II(d, a, b, c, x[ 3], 10, 0x8f0ccc92);
        II(c, d, a, b, x[10], 15, 0xffeff47d);
        II(b, c, d, a, x[ 1], 21, 0x85845dd1);
        II(a, b, c, d, x[ 8], 6, 0x6fa87e4f);
        II(d, a, b, c, x[15], 10, 0xfe2ce6e0);
        II(c, d, a, b, x[ 6], 15, 0xa3014314);
        II(b, c, d, a, x[13], 21, 0x4e0811a1);
        II(a, b, c, d, x[ 4], 6, 0xf7537e82);
        II(d, a, b, c, x[11], 10, 0xbd3af235);
        II(c, d, a, b, x[ 2], 15, 0x2ad7d2bb);
        II(b, c, d, a, x[ 9], 21, 0xeb86d391);

        state[0] += a;
        state[1] += b;
        state[2] += c;
        state[3] += d;

        // Zeroize sensitive information.
        std::memset(x, 0, sizeof(x));
        #undef FF
        #undef GG
        #undef HH
        #undef II
    }

    // Encodes input (uint32_t) into output (unsigned char). Assumes length is a multiple of 4.
    void encode(const uint32_t* input, unsigned char* output, std::size_t length) {
        for (std::size_t i = 0, j = 0; j < length; i++, j += 4) {
            output[j]     = static_cast<unsigned char>(input[i] & 0xff);
            output[j + 1] = static_cast<unsigned char>((input[i] >> 8) & 0xff);
            output[j + 2] = static_cast<unsigned char>((input[i] >> 16) & 0xff);
            output[j + 3] = static_cast<unsigned char>((input[i] >> 24) & 0xff);
        }
    }

    // Decodes input (unsigned char) into output (uint32_t). Assumes length is a multiple of 4.
    void decode(const unsigned char* input, uint32_t* output, std::size_t length) {
        for (std::size_t i = 0, j = 0; j < length; i++, j += 4) {
            output[i] = static_cast<uint32_t>(input[j]) |
                        (static_cast<uint32_t>(input[j + 1]) << 8) |
                        (static_cast<uint32_t>(input[j + 2]) << 16) |
                        (static_cast<uint32_t>(input[j + 3]) << 24);
        }
    }

    uint32_t state[4];         // State (ABCD).
    uint32_t count[2];         // Number of bits processed, modulo 2^64.
    unsigned char buffer[64];  // Input buffer.
    unsigned char digest[16];  // Final digest.
    bool finalized;
};

// Helper function to compute the MD5 hash of a given string.
std::string md5(const std::string &input) {
    MD5 context;
    context.update(reinterpret_cast<const unsigned char*>(input.c_str()), input.size());
    return context.final();
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Read the Door ID from standard input.
    std::string doorID;
    std::cin >> doorID;

    // Initialize the password with placeholder characters.
    const int passwordLength = 8;
    std::string password(passwordLength, '_');
    int filledPositions = 0;

    // Iterate over increasing indices until the password is complete.
    unsigned int index = 0;
    while (filledPositions < passwordLength) {
        // Build the candidate string: doorID concatenated with the current index.
        std::ostringstream candidate;
        candidate << doorID << index;
        std::string hash = md5(candidate.str());

        // Check if the hash starts with five zeroes.
        if (hash.compare(0, 5, "00000") == 0) {
            // The sixth character represents the position.
            char posChar = hash[5];
            // Ensure the position is a valid digit between '0' and '7'.
            if (posChar >= '0' && posChar <= '7') {
                int pos = posChar - '0';
                // Only fill the position if it hasn't been set yet.
                if (password[pos] == '_') {
                    password[pos] = hash[6];
                    ++filledPositions;

                    // Optional: print current progress.
                    std::cout << "\rDecrypting: " << password << std::flush;
                }
            }
        }
        ++index;
    }
    std::cout << std::endl << password << std::endl;
    return 0;
}

