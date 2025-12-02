#include <iostream>
#include <queue>

int main() {
    int count = 0;
    constexpr int window_size = 3;
    std::queue<int> window;

    int current;

    while (std::cin >> current) {
        if (window.size() < window_size) {
            window.push(current);
            continue;
        }

        // Compare new value to the one dropping out of the window
        if (current > window.front()) {
            ++count;
        }

        window.pop();
        window.push(current);
    }

    std::cout << count << '\n';
}
