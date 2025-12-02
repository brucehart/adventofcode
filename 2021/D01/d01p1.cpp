#include <iostream>
#include <limits>
#include <optional>

int main() {
    std::optional<int> prev;
    int count = 0;

    int current;
    while (std::cin >> current) {
        if (prev && current > *prev) {
            ++count;
        }
        prev = current;
    }

    std::cout << count << '\n';
}
