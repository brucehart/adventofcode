#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

// Entry point of the program
int main() {
    // Vector to store the input numbers in their original order
    std::vector<long long> numbers;
    long long num;

    // Read numbers from standard input until EOF
    while (std::cin >> num) {
        numbers.push_back(num);
    }

    // Initialize a list with the input numbers to allow efficient insertions and deletions
    std::list<long long> lst(numbers.begin(), numbers.end());

    // Vector to store iterators pointing to each element in the list, preserving original order
    std::vector<std::list<long long>::iterator> iterators;
    iterators.reserve(numbers.size());
    for (auto it = lst.begin(); it != lst.end(); ++it) {
        iterators.push_back(it);
    }

    // Perform the mixing process
    for (auto it : iterators) {
        long long value = *it;

        // Skip moving if the value is zero
        if (value == 0) {
            continue;
        }

        // Calculate the effective shift using modulo to handle large shifts and wrap around
        auto list_size = lst.size();
        long long shift = value % static_cast<long long>(list_size - 1);

        // Adjust shift for negative values to ensure it's positive
        if (shift < 0) {
            shift += (list_size - 1);
        }

        // If shift is zero after adjustment, no movement is needed
        if (shift == 0) {
            continue;
        }

        // Find the new position by advancing 'shift' steps from the current iterator
        auto pos = it;
        for (long long i = 0; i < shift; ++i) {
            ++pos;
            if (pos == lst.end()) {
                pos = lst.begin(); // Wrap around to the beginning if end is reached
            }
        }

        // To insert the element after 'shift' positions, advance 'pos' once more
        ++pos;
        if (pos == lst.end()) {
            pos = lst.begin(); // Wrap around if necessary
        }

        // Move the current element to the new position using splice
        lst.splice(pos, lst, it);
    }

    // Locate the iterator pointing to the value '0' in the mixed list
    auto zero_it = std::find(lst.begin(), lst.end(), 0);

    // If '0' is not found, output 0 and terminate
    if (zero_it == lst.end()) {
        std::cout << "0" << std::endl;
        return 0;
    }

    // Define the steps after '0' to find the grove coordinates
    std::vector<int> steps = {1000, 2000, 3000};
    long long sum = 0;

    // For each step, find the corresponding number after '0' and add it to the sum
    for (const auto& step : steps) {
        auto it = zero_it;
        for (int i = 0; i < step; ++i) {
            ++it;
            if (it == lst.end()) {
                it = lst.begin(); // Wrap around to the beginning if end is reached
            }
        }
        sum += *it;
    }

    // Output the final sum of the three grove coordinates
    std::cout << sum << std::endl;

    return 0;
}
