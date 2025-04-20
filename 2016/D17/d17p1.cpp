#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <array>
#include <algorithm>
#include <cstring>
#include <cstdint>

// Transformation operations
#define FF(a,b,c,d,x,s,ac) (a = rotate_left(a + F(b,c,d) + x + ac, s) + b)
#define GG(a,b,c,d,x,s,ac) (a = rotate_left(a + G(b,c,d) + x + ac, s) + b)
#define HH(a,b,c,d,x,s,ac) (a = rotate_left(a + H(b,c,d) + x + ac, s) + b)
#define II(a,b,c,d,x,s,ac) (a = rotate_left(a + I(b,c,d) + x + ac, s) + b)

// Minimal MD5 implementation based on RFC 1321 reference.
// Computes MD5 hash of input string and returns hex representation.
class MD5 {
public:
    MD5() { init(); }

    void update(const uint8_t* data, size_t len) {
        size_t i = 0, idx = (count[0] >> 3) & 0x3F;
        uint32_t part = 64 - idx;
        count[0] += static_cast<uint32_t>(len << 3);
        if (count[0] < (len << 3)) count[1]++;
        count[1] += static_cast<uint32_t>(len >> 29);

        if (len >= part) {
            std::memcpy(buffer + idx, data, part);
            transform(buffer);
            for (i = part; i + 63 < len; i += 64)
                transform(data + i);
            idx = 0;
        }
        if (i < len)
            std::memcpy(buffer + idx, data + i, len - i);
    }

    void finalize(uint8_t digest[16]) {
        static const uint8_t PADDING[64] = { 0x80 };
        uint8_t bits[8];
        encode(count, bits, 8);
        size_t idx = (count[0] >> 3) & 0x3F;
        size_t padLen = (idx < 56) ? (56 - idx) : (120 - idx);
        update(PADDING, padLen);
        update(bits, 8);
        encode(state, digest, 16);
    }

    static std::string hash(const std::string& input) {
        MD5 md5;
        md5.update(reinterpret_cast<const uint8_t*>(input.data()), input.size());
        uint8_t digest[16];
        md5.finalize(digest);
        static const char* HEX = "0123456789abcdef";
        std::string out;
        out.reserve(32);
        for (int i = 0; i < 16; ++i) {
            out.push_back(HEX[digest[i] >> 4]);
            out.push_back(HEX[digest[i] & 0xF]);
        }
        return out;
    }

private:
    void init() {
        count[0] = count[1] = 0;
        // RFC 1321 specifies these initial state values
        state[0] = 0x67452301;
        state[1] = 0xEFCDAB89;
        state[2] = 0x98BADCFE;
        state[3] = 0x10325476;
    }

    static uint32_t F(uint32_t x, uint32_t y, uint32_t z) { return (x & y) | (~x & z); }
    static uint32_t G(uint32_t x, uint32_t y, uint32_t z) { return (x & z) | (y & ~z); }
    static uint32_t H(uint32_t x, uint32_t y, uint32_t z) { return x ^ y ^ z; }
    static uint32_t I(uint32_t x, uint32_t y, uint32_t z) { return y ^ (x | ~z); }
    static uint32_t rotate_left(uint32_t x, uint32_t n) { return (x << n) | (x >> (32 - n)); }

    void transform(const uint8_t block[64]) {
        uint32_t a = state[0], b = state[1], c = state[2], d = state[3], x[16];
        decode(block, x, 64);

        // Round 1
        FF(a,b,c,d,x[ 0], 7,0xd76aa478); FF(d,a,b,c,x[ 1],12,0xe8c7b756);
        FF(c,d,a,b,x[ 2],17,0x242070db); FF(b,c,d,a,x[ 3],22,0xc1bdceee);
        FF(a,b,c,d,x[ 4], 7,0xf57c0faf); FF(d,a,b,c,x[ 5],12,0x4787c62a);
        FF(c,d,a,b,x[ 6],17,0xa8304613); FF(b,c,d,a,x[ 7],22,0xfd469501);
        FF(a,b,c,d,x[ 8], 7,0x698098d8); FF(d,a,b,c,x[ 9],12,0x8b44f7af);
        FF(c,d,a,b,x[10],17,0xffff5bb1); FF(b,c,d,a,x[11],22,0x895cd7be);
        FF(a,b,c,d,x[12], 7,0x6b901122); FF(d,a,b,c,x[13],12,0xfd987193);
        FF(c,d,a,b,x[14],17,0xa679438e); FF(b,c,d,a,x[15],22,0x49b40821);

        // Round 2
        GG(a,b,c,d,x[ 1], 5,0xf61e2562); GG(d,a,b,c,x[ 6],9,0xc040b340);
        GG(c,d,a,b,x[11],14,0x265e5a51); GG(b,c,d,a,x[ 0],20,0xe9b6c7aa);
        GG(a,b,c,d,x[ 5], 5,0xd62f105d); GG(d,a,b,c,x[10],9,0x02441453);
        GG(c,d,a,b,x[15],14,0xd8a1e681); GG(b,c,d,a,x[ 4],20,0xe7d3fbc8);
        GG(a,b,c,d,x[ 9], 5,0x21e1cde6); GG(d,a,b,c,x[14],9,0xc33707d6);
        GG(c,d,a,b,x[ 3],14,0xf4d50d87); GG(b,c,d,a,x[ 8],20,0x455a14ed);
        GG(a,b,c,d,x[13], 5,0xa9e3e905); GG(d,a,b,c,x[ 2],9,0xfcefa3f8);
        GG(c,d,a,b,x[ 7],14,0x676f02d9); GG(b,c,d,a,x[12],20,0x8d2a4c8a);

        // Round 3
        HH(a,b,c,d,x[ 5], 4,0xfffa3942); HH(d,a,b,c,x[ 8],11,0x8771f681);
        HH(c,d,a,b,x[11],16,0x6d9d6122); HH(b,c,d,a,x[14],23,0xfde5380c);
        HH(a,b,c,d,x[ 1], 4,0xa4beea44); HH(d,a,b,c,x[ 4],11,0x4bdecfa9);
        HH(c,d,a,b,x[ 7],16,0xf6bb4b60); HH(b,c,d,a,x[10],23,0xbebfbc70);
        HH(a,b,c,d,x[13], 4,0x289b7ec6); HH(d,a,b,c,x[ 0],11,0xeaa127fa);
        HH(c,d,a,b,x[ 3],16,0xd4ef3085); HH(b,c,d,a,x[ 6],23,0x04881d05);
        HH(a,b,c,d,x[ 9], 4,0xd9d4d039); HH(d,a,b,c,x[12],11,0xe6db99e5);
        HH(c,d,a,b,x[15],16,0x1fa27cf8); HH(b,c,d,a,x[ 2],23,0xc4ac5665);

        // Round 4
        II(a,b,c,d,x[ 0], 6,0xf4292244); II(d,a,b,c,x[ 7],10,0x432aff97);
        II(c,d,a,b,x[14],15,0xab9423a7); II(b,c,d,a,x[ 5],21,0xfc93a039);
        II(a,b,c,d,x[12], 6,0x655b59c3); II(d,a,b,c,x[ 3],10,0x8f0ccc92);
        II(c,d,a,b,x[10],15,0xffeff47d); II(b,c,d,a,x[ 1],21,0x85845dd1);
        II(a,b,c,d,x[ 8], 6,0x6fa87e4f); II(d,a,b,c,x[15],10,0xfe2ce6e0);
        II(c,d,a,b,x[ 6],15,0xa3014314); II(b,c,d,a,x[13],21,0x4e0811a1);
        II(a,b,c,d,x[ 4], 6,0xf7537e82); II(d,a,b,c,x[11],10,0xbd3af235);
        II(c,d,a,b,x[ 2],15,0x2ad7d2bb); II(b,c,d,a,x[ 9],21,0xeb86d391);

        state[0] += a; state[1] += b; state[2] += c; state[3] += d;
        std::fill(std::begin(x), std::end(x), 0);
    }

    static void encode(const uint32_t* src, uint8_t* dst, size_t len) {
        for (size_t i = 0, j = 0; j < len; ++i, j += 4) {
            dst[j]     = static_cast<uint8_t>( src[i]        & 0xFF);
            dst[j + 1] = static_cast<uint8_t>((src[i] >> 8 ) & 0xFF);
            dst[j + 2] = static_cast<uint8_t>((src[i] >> 16) & 0xFF);
            dst[j + 3] = static_cast<uint8_t>((src[i] >> 24) & 0xFF);
        }
    }

    static void decode(const uint8_t* src, uint32_t* dst, size_t len) {
        for (size_t i = 0, j = 0; j < len; ++i, j += 4) {
            dst[i] = (static_cast<uint32_t>(src[j])      ) |
                     (static_cast<uint32_t>(src[j+1])<< 8) |
                     (static_cast<uint32_t>(src[j+2])<<16) |
                     (static_cast<uint32_t>(src[j+3])<<24);
        }
    }



    uint32_t state[4];
    uint32_t count[2];
    uint8_t buffer[64];
};

struct Node {
    int x, y;
    std::string path;
};

int main() {
    std::string passcode;
    if (!std::getline(std::cin, passcode) || passcode.empty()) {
        return 0;
    }

    const int targetX = 3, targetY = 3;
    std::deque<Node> q;
    q.push_back({0, 0, ""});

    // Directions: Up, Down, Left, Right
    const std::array<char,4> moves = {'U','D','L','R'};
    const std::array<int,4> dx = {0, 0, -1, 1};
    const std::array<int,4> dy = {-1, 1, 0, 0};

    while (!q.empty()) {
        Node cur = q.front();
        q.pop_front();

        // Check if we've reached the vault
        if (cur.x == targetX && cur.y == targetY) {
            std::cout << cur.path << std::endl;
            return 0;
        }

        // Compute MD5 of passcode + path so far
        std::string hash = MD5::hash(passcode + cur.path);

        // Check each door in U,D,L,R order
        for (int i = 0; i < 4; ++i) {
            char h = hash[i];
            bool open = (h >= 'b' && h <= 'f'); // doors open if hex char b-f
            int nx = cur.x + dx[i], ny = cur.y + dy[i];
            // Ensure door is open and new position is within 4x4 grid
            if (open && nx >= 0 && nx < 4 && ny >= 0 && ny < 4) {
                q.push_back({nx, ny, cur.path + moves[i]});
            }
        }
    }

    // No path found (should not happen with valid input)
    std::cout << "NO PATH" << std::endl;
    return 0;
}
