#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>

struct Elem {
    int value;
    int depth;
};

static std::vector<Elem> parse_snail(const std::string& s) {
    std::vector<Elem> out;
    int depth = 0;
    for (std::size_t i = 0; i < s.size(); i++) {
        const char c = s[i];
        if (c == '[') {
            depth++;
        } else if (c == ']') {
            depth--;
        } else if (std::isdigit(static_cast<unsigned char>(c))) {
            int v = 0;
            std::size_t j = i;
            while (j < s.size() && std::isdigit(static_cast<unsigned char>(s[j]))) {
                v = v * 10 + (s[j] - '0');
                j++;
            }
            out.push_back({v, depth});
            i = j - 1;
        }
    }
    return out;
}

static bool explode(std::vector<Elem>& a) {
    for (std::size_t i = 0; i + 1 < a.size(); i++) {
        if (a[i].depth > 4 && a[i].depth == a[i + 1].depth) {
            if (i > 0) a[i - 1].value += a[i].value;
            if (i + 2 < a.size()) a[i + 2].value += a[i + 1].value;
            a[i] = {0, a[i].depth - 1};
            a.erase(a.begin() + static_cast<long long>(i + 1));
            return true;
        }
    }
    return false;
}

static bool split(std::vector<Elem>& a) {
    for (std::size_t i = 0; i < a.size(); i++) {
        if (a[i].value >= 10) {
            const int v = a[i].value;
            const int d = a[i].depth + 1;
            const int left = v / 2;
            const int right = v - left;
            a[i] = {left, d};
            a.insert(a.begin() + static_cast<long long>(i + 1), {right, d});
            return true;
        }
    }
    return false;
}

static void reduce(std::vector<Elem>& a) {
    while (true) {
        if (explode(a)) continue;
        if (split(a)) continue;
        break;
    }
}

static std::vector<Elem> add(std::vector<Elem> a, std::vector<Elem> b) {
    for (auto& e : a) e.depth++;
    for (auto& e : b) e.depth++;
    a.insert(a.end(), b.begin(), b.end());
    reduce(a);
    return a;
}

static long long magnitude(std::vector<Elem> a) {
    while (a.size() > 1) {
        int max_depth = 0;
        for (const auto& e : a) max_depth = std::max(max_depth, e.depth);
        for (std::size_t i = 0; i + 1 < a.size(); i++) {
            if (a[i].depth == max_depth && a[i + 1].depth == max_depth) {
                const long long v = 3LL * a[i].value + 2LL * a[i + 1].value;
                a[i] = {static_cast<int>(v), max_depth - 1};
                a.erase(a.begin() + static_cast<long long>(i + 1));
                break;
            }
        }
    }
    return a.empty() ? 0 : a[0].value;
}

int main() {
    std::string line;
    std::vector<Elem> sum;
    bool have_sum = false;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::vector<Elem> cur = parse_snail(line);
        if (!have_sum) {
            sum = std::move(cur);
            have_sum = true;
        } else {
            sum = add(std::move(sum), std::move(cur));
        }
    }

    if (!have_sum) return 0;
    std::cout << magnitude(sum) << std::endl;
    return 0;
}

