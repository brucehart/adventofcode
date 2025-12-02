#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> numbers;
    std::string line;

    while (std::cin >> line) numbers.push_back(line);
    if (numbers.empty()) return 0;

    auto find_rating = [](std::vector<std::string> candidates, bool oxygen) {
        const int width = candidates[0].size();

        for (int bit = 0; bit < width && candidates.size() > 1; ++bit) {
            int ones = 0;
            for (const auto& value : candidates) ones += value[bit] == '1';
            const int zeros = static_cast<int>(candidates.size()) - ones;

            const char keep = oxygen ? (ones >= zeros ? '1' : '0')
                                     : (zeros <= ones ? '0' : '1');

            std::vector<std::string> next;
            next.reserve(candidates.size());
            for (const auto& value : candidates)
                if (value[bit] == keep) next.push_back(value);

            candidates.swap(next);
        }

        int result = 0;
        for (char c : candidates.front()) result = (result << 1) | (c == '1');
        return result;
    };

    const int oxygen_rating = find_rating(numbers, true);
    const int co2_rating = find_rating(numbers, false);

    std::cout << oxygen_rating * co2_rating << std::endl;
}
